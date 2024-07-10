/*****************************************************************************
** File Name:      mmidc_guipda.c                                            *
** Author:                                                                   *
** Date:           07/16/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera pda                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011      robert.wang          Creat
******************************************************************************/

#define _MMIDC_GUIPDA_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#ifdef CAMERA_SUPPORT
#ifdef MMIDC_DESKTOP_PDA
#ifdef WIN32 
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_default.h"
#include "guiblock.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "block_mem.h"

#include "cafcontrol.h"
#include "guisetlist.h"
#include "guilistbox.h"
#include "guictrl_api.h"

#include "guiiconlist.h"
#include "mmk_tp.h"
#include "mmi_theme.h"
#include "mmi_appmsg.h"

#include "guires.h"
#include "guibutton.h"
#include "mmidc_main.h"

//other module
#include "mmiudisk_export.h"
#include "mmifmm_export.h"
#include "mmipic_edit_export.h"
#include "mmimultim_image.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmivp_internal.h"
#endif
#endif

//this module
#include "mmidc_export.h"
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_camera_image.h"
#include "mmidc_camera_anim.h"
#include "mmidc_setting.h"
#include "mmidc_display.h"
#include "mmidc_gui.h"
#include "mmidc_setting.h"
#include "mmidc_option.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif

#if 0//defined(MMIDC_F_EFFECT_360)
#include "mmidc_effect360.h"
#endif

#if defined(MMIDC_F_U_CAMERA)
#include "mmidc_ucamera_wintab.h"
#include "mmidc_imageprocess.h"
#endif

#ifdef MMIDC_F_N_IN_ONE
#include "mmidc_ninone.h"
#endif

#if defined(MMIDC_F_SMILE_AUTO_PHOTO)
#include "mmidc_smileautophoto.h"
#endif

#include "mmidc_guipda.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/
#define MMIDC_PDA_ICON_LIST_MAX_ITEM 5
//#define MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION
#define MMIDC_EXPAND_POINT_DIFF 5

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_DESKTOP_PDA_T  *s_desktop_pda_ptr = PNULL;
//LOCAL MMIDC_IMG_INFO_T s_preview_img_info = {0};
#ifdef MMIDC_F_LAST_IMAGE
    MMIDC_IMG_INFO_T* s_last_thumbnail_info_ptr = PNULL;
#endif

LOCAL const wchar s_photo_img_all_suffix[] = 
{
    L"*.jpg"
};

LOCAL const wchar s_video_img_all_suffix[] = 
{
    L"*.avi|*.3gp|*.mp4 "
};

LOCAL MMIDC_ZOOM_SEEKBAR_INFO_T s_zoom_seekbar_info = {0};
LOCAL MMIDC_IMAGE_ATTR_INFO_T *s_image_attr_info_ptr = PNULL;
LOCAL MMIDC_PDA_ICON_RECT_T s_pda_icon_rects = {0};
LOCAL PDA_MENU_KEY_STATE_E s_menu_key_state = 0;
LOCAL MMIDC_OSD_ANIM_T *s_osd_anim_ptr = PNULL;
LOCAL BUTTON_LIST_FOCUS_E s_button_list_focus = BUTTON_LIST_FOCUS_MODE;
LOCAL MMIDC_PDA_EXPAND_INFO_T s_pda_expand_info = {0};
LOCAL BOOLEAN  s_flag_whether_is_right = FALSE;
LOCAL uint32 s_cur_selection_left_icon = 0;


/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : set desktop visible or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetDesktopVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description : process zoom seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTimer(uint16 timer_id);

/*****************************************************************************/
//Description : display zoom seekbar down tp
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayZoomSeekBar(void);

/*****************************************************************************/
//Description : start zoom seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartZoomSeekBarTimer(void);

/*****************************************************************************/
//Description : handle zoom seek bar tp down
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpDown(GUI_POINT_T point);

/*****************************************************************************/
//Description : handle zoom seek bar tp up
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpUp(GUI_POINT_T point);

/*****************************************************************************/
//Description :  create pda desktop all items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void CreatePDAAllItems(void);

/*****************************************************************************/
//Description : init zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitZoomSeekBarInfo(BOOLEAN is_hor_draw);

/*****************************************************************************/
//Description : destroy image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DestroyImageAttrInfo(void);


/*****************************************************************************/
//Description : handle image attribute seek bar tp down
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpDown(GUI_POINT_T point);

/*****************************************************************************/
//Description : handle image attribute seek bar tp up
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpUp(GUI_POINT_T point);

/*****************************************************************************/
//Description : display image attribute seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayImageAttrAllSeekBar(void);

/*****************************************************************************/
//Description : handle image attribute seek bar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTimer(uint16 timer_id);

/*****************************************************************************/
//Description : init image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitImageAttrInfo(void);

/*****************************************************************************/
//Description : create image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateImageAttrInfo(void);

/*****************************************************************************/
//Description :  destroy icon type list ctrl
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DestroyIconTypeListCtrl(void);

/*****************************************************************************/
//Description :  reset expand button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetExpandBtn(void);

/*****************************************************************************/
//Description :  reset zoom button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetZoomBtn(void);

/*****************************************************************************/
//Description :  create expand button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateExpandBtn(void);

/*****************************************************************************/
//Description : get zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_ZOOM_SEEKBAR_INFO_T GetZoomSeekBarInfo(void);

/*****************************************************************************/
//Description : create expand panel
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateExpandPanel(void);

/*****************************************************************************/
//Description : display all cmd buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayAllCmdBtns(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//Description :  display cmd buttons image
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayCmdBtns(void);

/*****************************************************************************/
//Description : set menu key state
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetMenuKeyState(PDA_MENU_KEY_STATE_E  key_state);

/*****************************************************************************/
//Description : get menu key state
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL PDA_MENU_KEY_STATE_E GetMenuKeyState(void);

/*****************************************************************************/
//Description : set as a wallpaper
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetWallpaper(void);

#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//Description : edit pic
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void EditPic(void);
#endif
/*****************************************************************************/
//Description : handle Web key in PDA Camera;
//Global resource dependence : 
//Author: glen.li
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleWebKeyInPDA(MMI_MESSAGE_ID_E msg_id);
//Description : handle direction key in PDA Camera;
//Global resource dependence : 
//Author: glen.li
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleDirectionKeyInPDA(MMI_MESSAGE_ID_E msg_id); 
//Description : handle pda key
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePDAKey(MMI_MESSAGE_ID_E msg_id, //IN
                           void *param //in
                           );

/*****************************************************************************/
//Description : set expand panel visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetExpandPanelVisible(BOOLEAN is_visible);
//#ifdef PIC_VIEWER_SUPPORT
/*****************************************************************************/
//Description :  set pic btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPicBtnVisible(BOOLEAN is_visible);
//#endif
/*****************************************************************************/
//Description :  set zoom btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetZoomBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  set sensor btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetSensorBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  set capture btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetCaptureBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  reset capture button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetCaptureBtn(CAMERA_MODE_E mode);
/*****************************************************************************/
//Description : DrawBrightnessBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawBrightnessBmp(void);

/*****************************************************************************/
//Description : DrawContrastBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawContrastBmp(void);

/*****************************************************************************/
//Description : DrawEcBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawEcBmp(void);

/*****************************************************************************/
//Description : DrawPicPropertyBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawPicPropertyBmp(void);

/*****************************************************************************/
//Description : DrawPicBoundaryBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawPicBoundaryBmp(void);

/*****************************************************************************/
//  Description : DrawPicPropertyBackground
//  Global resource dependence : none
//  Author: kunliu
//  Note:
/*****************************************************************************/
LOCAL void DrawPicPropertyBackground(void);
/*****************************************************************************/
//Description : draw seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DrawSeekBar(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                           );
/*****************************************************************************/
//Description : get image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_IMAGE_ATTR_INFO_T* GetImageAttrInfoByPtr(void);

#ifdef MMIDC_F_CAPTURE_ANIM
/*****************************************************************************/
//Description :  start capture anim before
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartCaptureAnimBefore(void);

/*****************************************************************************/
//Description :  start capture anim after
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartCaptureAnimAfter(void);
#endif

/*****************************************************************************/
//Description : restore pda desktop
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void RestorePDADesktop(void);

/*****************************************************************************/
//Description : set zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetZoomSeekBarInfo(MMIDC_ZOOM_SEEKBAR_INFO_T *seekbar_info_ptr //in
                              );

/*****************************************************************************/
//Description : draw seekbar scale
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawSeekBarScale(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                               );
/*****************************************************************************/
//Description :  reset Setting button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetSettingBtn(void);
/*****************************************************************************/
//Description :  reset Effect button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetEffectBtn(void);
/*****************************************************************************/
//Description :  reset ImageProperty button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetImagePropertyBtn(void);
/*****************************************************************************/
//Description :  reset WhiteBlance button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetWhiteBlanceBtn(void);
/*****************************************************************************/
//Description :  reset mode button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetModeBtn(void);
/*****************************************************************************/
//Description :  reset buttonlist 
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetButtonList(void);

/*****************************************************************************/
//Description :  create  buttonlist
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateButtonList(void);

/*****************************************************************************/
//Description :  set buttonlist visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetButtonListVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  set expand btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetExpandBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description : handle zoom seek bar tp move
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpMove(GUI_POINT_T point);

/*****************************************************************************/
//Description : handle image attribute seek bar tp move
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpMove(GUI_POINT_T point);

/*****************************************************************************/
//Description : stop image attribute seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopImageAttrSeekBarTimer(void);

/*****************************************************************************/
//Description : stop zoom seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopZoomSeekBarTimer(void);

/*****************************************************************************/
//Description :  reset sensor button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetSensorBtn(void);
//#ifdef PIC_VIEWER_SUPPORT
/*****************************************************************************/
//Description :  reset pic button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetPicBtn(void);
//#endif
/*****************************************************************************/
//Description :  set arrow left btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetArrowLeftBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  reset arrow left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetArrowLeftBtn(BOOLEAN is_gray
                             );

/*****************************************************************************/
//Description :  set arrow right btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetArrowRightBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  reset arrow right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetArrowRightBtn(BOOLEAN is_gray
                              );
/*****************************************************************************/
//Description :  clrar list rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearListRect(void);
/*****************************************************************************/
//Description :  clear arrow btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ClearArrowBtnRect(void);
/*****************************************************************************/
//Description : display arrow buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayArrowBtns(void);
/*****************************************************************************/
//Description : handle glide expand tp down
//Global resource dependence : 
//Author: kunliu  
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGlideExpandTpDown(GUI_POINT_T point);

/*****************************************************************************/
//Description : handle glide expand tp up
//Global resource dependence : 
//Author: kunliu  
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGlideExpandTpUp(GUI_POINT_T point);
/*****************************************************************************/
//Description : hide expand button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HideExpandPanelBtn(MMI_HANDLE_T ctrl_id);
/*****************************************************************************/
//Description :  clrar expand rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearExpandPanelRect(void);

/*****************************************************************************/
//Description : SetImageAttrAllSeekBar
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN SetImageAttrAllSeekBar(int16 move_len, BOOLEAN is_press);

/*****************************************************************************/
//Description : reset image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetImageAttrInfo(void);

#if 0//defined(MMIDC_F_EFFECT_360)
/*****************************************************************************/
//Description : display effect 360 arrow buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayEffect360ArrowBtns(void);

/*****************************************************************************/
//Description :  create effect 360 left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateEffect360LeftBtn(void);

/*****************************************************************************/
//Description :  create effect 360 right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateEffect360RightBtn(void);

/*****************************************************************************/
//Description :  set effect 360 left btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetEffect360LeftBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description :  set effect 360 right btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetEffect360RightBtnVisible(BOOLEAN is_visible);

/*****************************************************************************/
//Description : process effect 360 left btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessEffect360LeftBtn(void);

/*****************************************************************************/
//Description : process effect 360 right btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessEffect360RightBtn(void);
#endif //#if defined(MMIDC_F_EFFECT_360)

/*****************************************************************************/
//Description :  get button Setting both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnSettingBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              );

#if defined(MMIDC_F_U_CAMERA)
/*****************************************************************************/
//Description :  reset u camera button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetUCameraBtn(void);

/*****************************************************************************/
//Description :  set ucamera btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetUCameraBtnVisible(BOOLEAN is_visible);

#endif

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description : start zoom seekbar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartZoomSeekBarDisplayTimer(void);

/*****************************************************************************/
//Description : stop zoom seekbar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopZoomSeekBarDisplayTimer(void);

/*****************************************************************************/
//Description : is display seek bar in recording
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplaySeekBar(void);

/*****************************************************************************/
//Description : set display seek bar flag in recording
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetDisplaySeekBarFlag(BOOLEAN flag);

/*****************************************************************************/
//Description : handle zoom seek bar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarDisplayTimer(uint16 timer_id);

/*****************************************************************************/
//Description : clear seek bar rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearSeekBarRect(void);

/*****************************************************************************/
//Description : is point in cmd rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPointInCmdRect(GUI_POINT_T point);

/*****************************************************************************/
//Description : handle other msg to display seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void HandleOtherMsgToDisplaySeekBar(BOOLEAN ret, //in
                                                       MMI_MESSAGE_ID_E msg_id, //in
                                                       void *param //in
                                                       );
#endif

#ifdef MMIDC_F_LAST_IMAGE
/*****************************************************************************/
//Description :  get last thumbnail rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLastThumbnailRect(void);

/*****************************************************************************/
//Description :  last thumbnail owner draw fun
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL void LastThumbnailOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
//Description :  display last thumbnail
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayLastThumbnail(void);

/*****************************************************************************/
//Description :  get last thumbnail data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_GET_LAST_THUMBNAIL_DATA_E GetLastThumbnailData(CAMERA_MODE_E camera_mode);

/*****************************************************************************/
//Description : get last thumbnail decoded data
//Global resource dependence : 
//Author: chunyou
//Note: 

/*****************************************************************************/
LOCAL MMIDC_GET_LAST_THUMBNAIL_DATA_E GetLastThumbnailDecodedData(CAMERA_MODE_E camera_mode,//in
                                                    GUIANIM_FILE_INFO_T  *file_info_ptr//in
                                                    );

/****************************************************************************/
//Description :  clear last thumbnail rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearLastThumbnailRect(void);
#endif //MMIDC_F_LAST_IMAGE

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//Description :  get buttonlist focus
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BUTTON_LIST_FOCUS_E GetButtonListFocus(void)
{
    return s_button_list_focus;
}
/*****************************************************************************/
//Description :  set buttonlist focus
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetButtonListFocus(BUTTON_LIST_FOCUS_E focus)
{
    s_button_list_focus = focus;
}

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//Description :  get ctrl visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCtrlVisible(MMI_CTRL_ID_T ctrl_id)
{
    GUICTRL_STATE_T state = 0;
    
    BOOLEAN is_visible = FALSE;
    
    state = GUICTRL_STATE_INVISIBLE;

    is_visible = GUIAPICTRL_GetState(ctrl_id, state);
    is_visible = !is_visible;

    return is_visible;
}

/*****************************************************************************/
//Description : SetCtrlVisible
//Global resource dependence : 
//Author:robert.wang
//Note: 
/*****************************************************************************/
LOCAL void SetCtrlVisible(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_visible)
{
    GUICTRL_STATE_T state = 0;
    
    state = GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP;

    GUIAPICTRL_SetState(ctrl_id,
        state,
        !is_visible?TRUE:FALSE
        );
}

/*****************************************************************************/
//Description : Get Item Fixed Rect
//Global resource dependence : 
//Author:robert.wang
//Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetItemFixedRect(MMIDC_PDA_ICON_RECT_E item,
                                       BOOLEAN is_visible
                                       )
{
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_IMAGE_ID_T img_id = IMG_ICONLIST_PHOTO;
    uint16 width = 0;
    uint16 height = 0;

    int16 left = 0;
    int16 right = 0;
    int16 top = 0;
    int16 bottom = 0;
    
    GUI_RECT_T rect = {0};
    GUI_RECT_T pre_rect = {0};

    
    switch(item)
    {
    case PDA_RECT_BTN_LIST_BG:
        img_id = IMG_ICONLIST_PHOTO;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            if (is_visible)
            {
                left = BTNLIST_BG_SHOW_MODE_V_LEFT;
                right = BTNLIST_BG_SHOW_MODE_V_RIGHT;
                top = BTNLIST_BG_SHOW_MODE_V_TOP;
                bottom = BTNLIST_BG_SHOW_MODE_V_BOTTEOM;
            }
            else
            {
                left = BTNLIST_BG_HIDE_MODE_V_LEFT;
                right = BTNLIST_BG_HIDE_MODE_V_RIGHT;
                top = BTNLIST_BG_HIDE_MODE_V_TOP;
                bottom = BTNLIST_BG_HIDE_MODE_V_BOTTEOM;
            }
        }
        else
        {
            if (is_visible)
            {
                left = BTNLIST_BG_SHOW_MODE_H_LEFT;
                right = BTNLIST_BG_SHOW_MODE_H_RIGHT;
                top = BTNLIST_BG_SHOW_MODE_H_TOP;
                bottom = BTNLIST_BG_SHOW_MODE_H_BOTTEOM;
            }
            else
            {
                left = BTNLIST_BG_HIDE_MODE_H_LEFT;
                right = BTNLIST_BG_HIDE_MODE_H_RIGHT;
                top = BTNLIST_BG_HIDE_MODE_H_TOP;
                bottom = BTNLIST_BG_HIDE_MODE_H_BOTTEOM;
            }
        }
        break;
    case PDA_RECT_LIST_SHADOW:
        
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            img_id = IMG_ICONLIST_SHADOW_V;
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

            if (is_visible)
            {
                left = PIC_BOUNDARY_SHOW_RECT_V_LEFT;
                top = PIC_BOUNDARY_SHOW_RECT_V_TOP;
            }
            else
            {
                left = PIC_BOUNDARY_HIDE_RECT_V_LEFT;
                top = PIC_BOUNDARY_HIDE_RECT_V_TOP;
            }
        }
        else
        {
            img_id = IMG_ICONLIST_SHADOW;
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

            if (is_visible)
            {
                left = PIC_BOUNDARY_SHOW_RECT_H_LEFT;
                top = PIC_BOUNDARY_SHOW_RECT_H_TOP;
            }
            else
            {
                left = PIC_BOUNDARY_HIDE_RECT_LEFT;
                top = PIC_BOUNDARY_HIDE_RECT_TOP;
            }
        }
        
        right = left + width - 1;
        bottom = top + height - 1;

        break;
    case PDA_RECT_LABEL_RECT:
        img_id = IMAGE_CONTROL_LIST_SPLIT_ITEM_BG;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            if (is_visible)
            {
                left = LABEL_SHOW_RECT_V_LEFT;
                right = LABEL_SHOW_RECT_V_RIGHT;
                top = LABEL_SHOW_RECT_V_TOP;
                //bottom = LABEL_SHOW_RECT_V_BOTTOM;
            }
            else
            {
                left = LABEL_HIDE_RECT_V_LEFT;
                right = LABEL_HIDE_RECT_V_RIGHT;
                top = LABEL_HIDE_RECT_V_TOP;
                //bottom = LABEL_HIDE_RECT_V_BOTTOM;
            }
        }
        else
        {
            if (is_visible)
            {
                left = LABEL_SHOW_RECT_H_LEFT;
                right = LABEL_SHOW_RECT_H_RIGHT;
                top = LABEL_SHOW_RECT_H_TOP;
                //bottom = LABEL_SHOW_RECT_H_BOTTOM;
            }
            else
            {
                left = LABEL_HIDE_RECT_H_LEFT;
                right = LABEL_HIDE_RECT_H_RIGHT;
                top = LABEL_HIDE_RECT_H_TOP;
                //bottom = LABEL_HIDE_RECT_H_BOTTOM;
            }
        }
        bottom = top + height - 1;

        break;
    case PDA_RECT_LIST_RECT_BG:
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            if (is_visible)
            {
                left = LIST_SHOW_RECT_V_LEFT;
                right = LIST_SHOW_RECT_V_RIGHT;
                top = LIST_SHOW_RECT_V_TOP;
                bottom = LIST_SHOW_RECT_V_BOTTOM;
            }
            else
            {
                left = LIST_HIDE_RECT_V_LEFT;
                right = LIST_HIDE_RECT_V_RIGHT;
                top = LIST_HIDE_RECT_V_TOP;
                bottom = LIST_HIDE_RECT_V_BOTTOM;
            }

        }
        else
        {
            if (is_visible)
            {
                left = LIST_SHOW_RECT_H_LEFT;
                right = LIST_SHOW_RECT_H_RIGHT;
                top = LIST_SHOW_RECT_H_TOP;
                bottom = LIST_SHOW_RECT_H_BOTTOM;
            }
            else
            {
                left = LIST_HIDE_RECT_H_LEFT;
                right = LIST_HIDE_RECT_RIGHT;
                top = LIST_HIDE_RECT_TOP;
                bottom = LIST_HIDE_RECT_BOTTOM;
            }
        }
        break;
    case PDA_RECT_PIC_PROPERTY_BRIGHTNESS_ICON:
        img_id = IMG_BRIGHTNESS_ICON;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            if (is_visible)
            {
                left = PIC_PROPERTY_BRIGHTNESS_V_X;
                top = PIC_PROPERTY_BRIGHTNESS_V_Y;
            }
            else
            {
                left = HIDE_PIC_PROPERTY_BRIGHTNESS_V_X;
                top = HIDE_PIC_PROPERTY_BRIGHTNESS_V_Y;
            }
        }
        else
        {
            if (is_visible)
            {
                left = PIC_PROPERTY_BRIGHTNESS_H_X;
                top = PIC_PROPERTY_BRIGHTNESS_H_Y;
            }
            else
            {
                left = HIDE_PIC_PROPERTY_BRIGHTNESS_H_X;
                top = HIDE_PIC_PROPERTY_BRIGHTNESS_H_Y;
            }
        }
        
        right = left + width - 1;
        bottom = top + height - 1;

        break;
    case PDA_RECT_PIC_PROPERTY_CONTRAST_ICON:
        img_id = IMG_BRIGHTNESS_ICON;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        pre_rect = GetItemFixedRect(PDA_RECT_PIC_PROPERTY_BRIGHTNESS_ICON, is_visible);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            left = pre_rect.right + PIC_PROPERTY_ICON_V_SPACE;
        }
        else
        {
            left = pre_rect.right + PIC_PROPERTY_ICON_H_SPACE;
        }
        
        right = left + width - 1;
        top = pre_rect.top;
        bottom = top + height - 1;

        break;  
    case PDA_RECT_PIC_PROPERTY_EC_ICON:
        img_id = IMG_BRIGHTNESS_ICON;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        pre_rect = GetItemFixedRect(PDA_RECT_PIC_PROPERTY_CONTRAST_ICON, is_visible);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            left = pre_rect.right + PIC_PROPERTY_ICON_V_SPACE;
        }
        else
        {
            left = pre_rect.right + PIC_PROPERTY_ICON_H_SPACE;
        }
        right = left + width - 1;
        top = pre_rect.top;
        bottom = top + height - 1;
        
        break; 
    case PDA_RECT_PIC_PROPERTY_DV_BRIGHTNESS_ICON:
        img_id = IMG_BRIGHTNESS_ICON;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            if (is_visible)
            {
                left = DV_PIC_PROPERTY_BRIGHTNESS_V_X;
                top = DV_PIC_PROPERTY_BRIGHTNESS_V_Y;
            }
            else
            {
                left = DV_HIDE_PIC_PROPERTY_BRIGHTNESS_V_X;
                top = DV_HIDE_PIC_PROPERTY_BRIGHTNESS_V_Y;
            }
        }
        else
        {
            if (is_visible)
            {
                left = DV_PIC_PROPERTY_BRIGHTNESS_H_X;
                top = DV_PIC_PROPERTY_BRIGHTNESS_H_Y;
            }
            else
            {
                left = DV_HIDE_PIC_PROPERTY_BRIGHTNESS_H_X;
                top = DV_HIDE_PIC_PROPERTY_BRIGHTNESS_H_Y;
            }
        }
        
        right = left + width - 1;
        bottom = top + height - 1;

        break;
    case PDA_RECT_PIC_PROPERTY_DV_CONTRAST_ICON:
        img_id = IMG_BRIGHTNESS_ICON;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        pre_rect = GetItemFixedRect(PDA_RECT_PIC_PROPERTY_DV_BRIGHTNESS_ICON, is_visible);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            left = pre_rect.right + DV_PIC_PROPERTY_ICON_V_SPACE;
        }
        else
        {
            left = pre_rect.right + DV_PIC_PROPERTY_ICON_H_SPACE;
        }
        
        right = left + width - 1;
        top = pre_rect.top;
        bottom = top + height - 1;
        
        break; 
    case PDA_RECT_PIC_PROPERTY_SEEKBAR_BG:
        img_id = IMG_PIC_PROPERTY_BG;
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
             if (is_visible)
            {
                left = PIC_PROPERTY_BG_V_X;
                top = PIC_PROPERTY_BG_V_Y;
            }
            else
            {
                left = HIDE_PIC_PROPERTY_BG_V_X;
                top = HIDE_PIC_PROPERTY_BG_V_Y;
            }
        }
        else
        {
            if (is_visible)
            {
                left = PIC_PROPERTY_BG_H_X;
                top = PIC_PROPERTY_BG_H_Y;
            }
            else
            {
                left = HIDE_PIC_PROPERTY_BG_H_X;
                top = HIDE_PIC_PROPERTY_BG_H_Y;
            }
        }
        
        right = left + width - 1;
        bottom = top + height - 1;
        break;    
    case PDA_RECT_EXPAND:
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            img_id = IMG_ICONLIST_EXPAND_OUT_V;
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

            if (is_visible)
            {
                left = BTN_SHOW_EXPAND_V_LEFT;
                top = BTN_SHOW_EXPAND_V_TOP;
            }
            else
            {
                left = BTN_HIDE_EXPAND_V_LEFT;
                //top = BTNLIST_BG_HIDE_MODE_V_BOTTEOM;//BTN_HIDE_EXPAND_V_TOP;
				top = BTN_HIDE_EXPAND_V_TOP;
            }
        }
        else
        {
            img_id = IMG_ICONLIST_EXPAND_OUT;
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

            if (is_visible)
            {
                left = BTN_SHOW_EXPAND_H_LEFT;
                top = BTN_SHOW_EXPAND_H_TOP;
            }
            else
            {
                left = BTN_HIDE_EXPAND_H_LEFT;
                top = BTN_HIDE_EXPAND_H_TOP;
            }
        }
        
        right = left + width - 1;
        bottom = top + height - 1;

        break;        
    default:
        break;
    }
    
    rect.left = left;
    rect.right = right;
    rect.top = top;
    rect.bottom = bottom;
    
    return rect;
}
/*****************************************************************************/
//Description : get pda icon rect info by ptr
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_PDA_ICON_RECT_T* GetPDAIconRectsInfoByPtr(void)
{
    MMIDC_PDA_ICON_RECT_T *info_ptr = PNULL;
    
    info_ptr = &s_pda_icon_rects;
    
    return info_ptr;
}
/*****************************************************************************/
//Description : get icon rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIconRect(MMIDC_PDA_ICON_RECT_E  index)
{
    MMIDC_PDA_ICON_RECT_T *info_ptr = GetPDAIconRectsInfoByPtr();
    GUI_RECT_T rect = {0};
    
    if (PNULL != info_ptr)
    {
        rect = info_ptr->icon_info[index].pic_info.cur_rect;
    }
    return rect;
}

/*****************************************************************************/
//Description : get icon rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetIconRect(MMIDC_PDA_ICON_RECT_E  index,
                       GUI_RECT_T rect
                       )
{
    MMIDC_PDA_ICON_RECT_T *info_ptr = GetPDAIconRectsInfoByPtr();
    
    if (PNULL != info_ptr)
    {
        info_ptr->icon_info[index].pic_info.cur_rect = rect;
    }    
}

/*****************************************************************************/
//Description : get moved rect and change rect
//Global resource dependence : none
//Author: robert.wang
//Note: move_len maybe nagevte digital
/*****************************************************************************/
LOCAL GUI_RECT_T GetMovedRectAndChangeRect(MMIDC_ITEM_CONTENT_T content,
                                                       int16 move_len
                                                       )
{
    MMIDC_ITEM_TYPE_E item_type = 0;
    GUI_RECT_T rect = {0};
    SCREEN_MODE_E is_hor = MMIDC_GetScreenMode();
    uint32 index = 0;

    item_type = content.type;
    if (MMIDC_ITEM_TYPE_CTRL == item_type)
    {
        index = content.item_data.ctrl_id;
        GUIAPICTRL_GetRect(index, &rect);
    }
    else
    {
        index = content.item_data.index;
        rect = GetIconRect(index);
    }

    if (SCREEN_MODE_HORIIZONTAL == is_hor)
    {
        rect.left += move_len;
        rect.right += move_len;
    }
    else
    {
        rect.top += move_len;
        rect.bottom += move_len;
    }

    //self draw item need to save rect
    //ctrl item save rect by system
    if (MMIDC_ITEM_TYPE_CTRL == item_type)
    {
        GUIAPICTRL_SetRect(index, &rect);
    }
    else
    {
        SetIconRect(index, rect);
    }
    
    return rect;
}

/*****************************************************************************/
//Description : get icon state as press
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIconStateAsPress(MMIDC_PDA_ICON_RECT_E  index)
{
    MMIDC_PDA_ICON_RECT_T *info_ptr = GetPDAIconRectsInfoByPtr();
    BOOLEAN is_press = FALSE;
    
    if (PNULL != info_ptr)
    {
        is_press = info_ptr->icon_info[index].is_press;
    }
    
    return is_press;
}

/*****************************************************************************/
//Description : set icon state as press
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetIconStateAsPress(MMIDC_PDA_ICON_RECT_E  index,
                               BOOLEAN is_press
                               )
{
    MMIDC_PDA_ICON_RECT_T *info_ptr = GetPDAIconRectsInfoByPtr();
    
    if (PNULL != info_ptr)
    {
        info_ptr->icon_info[index].is_press = is_press;
    }
}

/*****************************************************************************/
//Description : get list ctrl item by ptr
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL  MMIDC_LIST_CTRL_ITEM_T * GetListCtrlItemByPtr(void)
{
    MMIDC_LIST_CTRL_ITEM_T *item_ptr = PNULL;
    
    if (PNULL != s_desktop_pda_ptr)
    {
        item_ptr = &s_desktop_pda_ptr->pda_item.list_item;
    }
    
    return item_ptr;
}

/*****************************************************************************/
//Description : get list ctrl item
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL  void GetListCtrlItem(MMIDC_LIST_CTRL_ITEM_T *item_ptr //out
                            )
{
    if (PNULL != s_desktop_pda_ptr)
    {
        *item_ptr = s_desktop_pda_ptr->pda_item.list_item;
    }
}

/*****************************************************************************/
//Description : get list ctrl item
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetListCtrlItem(MMIDC_LIST_CTRL_ITEM_T* item_ptr //in
                       )
{
    if (PNULL != s_desktop_pda_ptr)
    {
        s_desktop_pda_ptr->pda_item.list_item = *item_ptr;
    }
}

/*****************************************************************************/
//Description : add list item
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void AddListCtrlItem(MMI_HANDLE_T ctrl_id, //in
                           MMIDC_LIST_CTRL_ITEM_T *list_ptr //in
                           )
{
    uint16 i = 0;
    MMI_TEXT_ID_T       text_id = 0;
    MMI_TEXT_ID_T       text1_id = 0;
    MMI_IMAGE_ID_T      image_id = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    uint16 max_item_num = 0;
    uint16 cur_item = 0;
    MMI_TEXT_ID_T  title_id = TXT_NULL;
    MMI_TEXT_ID_T  *array_ptr = PNULL;
    MMI_TEXT_ID_T  *array1_ptr = PNULL;
    MMI_IMAGE_ID_T *array_image_ptr = PNULL;
    MMI_STRING_T *str_ptr = PNULL;
    ITEM_STRING_TYPE_E *str_type_ptr = PNULL;
    MMIDC_PDA_ICON_E icon_id = PDA_ICON_MAX;
    ITEM_STRING_TYPE_E string_type = 0;
    MMI_STRING_T text_str = {0};
    BOOLEAN is_gray = FALSE;
    uint16 item_index = 0;
    
    if (PNULL == list_ptr)
    {
        return;
    }
    
    title_id = list_ptr->title_text_id;
    max_item_num = list_ptr->max_item_num;
    cur_item = list_ptr->cur_item;
    cur_item += 1;
    array_ptr = list_ptr->text_array;
    array1_ptr = list_ptr->text1_array;
    array_image_ptr = list_ptr->image_array;
    str_ptr = list_ptr->string_array;
    str_type_ptr = list_ptr->string_type_array;
    icon_id = list_ptr->icon_id;
    
    //GUILIST_SetOwnSofterKey(ctrl_id,TRUE);    
    
    
    GUILIST_SetMaxItem(ctrl_id, max_item_num + 1, FALSE);
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = title_id;    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;

    item_t.item_state |=GUIITEM_STATE_SPLIT;

    item_t.item_data_ptr = &item_data;

    GUILIST_InsertItem(ctrl_id, &item_t, 0);
    
    //set list ctrl item data
    if(icon_id == PDA_ICON_SETTING)
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO_EXT;
    }
    item_t.item_data_ptr = &item_data;
    item_t.item_state = 0;
    
    
    for(i = 0; i < max_item_num; i++)
    {
        item_t.user_data = list_ptr->user_data[i];
        
        string_type = str_type_ptr[i];   
        switch(string_type)
        {
        case STRING_TYPE_NONE:
            text_id = array_ptr[i];        
            if (TXT_NULL != text_id)
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = text_id;
            }
            
            
            text1_id = array1_ptr[i];        
            if (TXT_NULL != text1_id)
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[2].item_data.text_id = text1_id;
            }
            
            break;
        case STRING_TYPE_STRING1:
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = str_ptr[i].wstr_len;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_ptr[i].wstr_ptr;
            break;
        case STRING_TYPE_STRING2:
            text_id = array_ptr[i];        
            if (TXT_NULL != text_id)
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = text_id;
            }
            
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_len = str_ptr[i].wstr_len;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = str_ptr[i].wstr_ptr;
            break;
        case STRING_TYPE_TEXT_SIZE:
            if (TXT_COMM_SIZE == title_id)
            {
                text_id = array_ptr[i];        
                if (TXT_NULL != text_id)
                {
                    MMIDC_GetSizeString((uint16)text_id, &text_str);
                    
                    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[0].item_data.text_buffer.wstr_len = text_str.wstr_len;
                    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
                }
            }
            break;
        default:
            break;
        }
        
        
        image_id = array_image_ptr[i];        
        if (TXT_NULL != image_id)
        {
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[1].item_data.image_id = image_id;
        }

        item_index = i + 1;
        if (!GUILIST_InsertItem(ctrl_id, &item_t, item_index))
        {               
            break;
        }        

        is_gray = list_ptr->gray_array[i];
        if (is_gray)
        {
            GUILIST_SetItemGray(ctrl_id, item_index, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, item_index, TRUE);
        }
    }
    
    //   GUILIST_SetBgColor(ctrl_id, MMI_BLACK_COLOR);
    
    //set focus item
    //if(icon_id != PDA_ICON_SETTING)
    {
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
    }
    
}
/*****************************************************************************/
//  Description : Anim
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateLabel(MMI_CTRL_ID_T ctrl_id)
{
    GUI_RECT_T   rect = {0};
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_BG_T    label_bg = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T    init_data = {0};
    
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();

    rect = GetItemFixedRect(PDA_RECT_LABEL_RECT, TRUE);

    
    MMK_DestroyControl(ctrl_id);
    init_data.both_rect.v_rect = rect;
    init_data.both_rect.h_rect = rect;
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_LABEL_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    MMK_CreateControl( &create );
    MMK_SetCtrlLcdDevInfo(ctrl_id, &dev_info);

    #if defined(MMIDC_F_PDA_BIT_16)
        label_bg.bg_type = GUI_BG_COLOR;
        label_bg.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SPLIT_LINE);
    #else
        label_bg.bg_type = GUI_BG_IMG;
        label_bg.img_id = IMAGE_CONTROL_LIST_SPLIT_ITEM_BG;
    #endif
    
    GUILABEL_SetBg(ctrl_id,&label_bg);
    GUILABEL_SetTextById(ctrl_id, TXT_DC_IMAGE_PROPERTY, TRUE);    
    GUILABEL_SetFont(ctrl_id, MMI_LIST_ITEM_SMALL_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT));

    MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
}
/*****************************************************************************/
//Description : create list ctrl
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateListCtrl(MMI_CTRL_ID_T ctrl_id, GUILIST_TYPE_E data_type
                          )
{
    GUI_RECT_T   rect = {0};
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMI_CONTROL_CREATE_T    create = {0};
    GUILIST_INIT_DATA_T init_data = {0};
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    BOOLEAN is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    BOOLEAN is_visible = FALSE;

    if(is_press)
    {
        is_visible = TRUE;
    }    
    rect = GetItemFixedRect(PDA_RECT_LIST_RECT_BG, is_visible);
    
    init_data.both_rect.v_rect = rect;
    init_data.both_rect.h_rect = rect;
    init_data.type = data_type;//GUILIST_RADIOLIST_E;
    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_LIST_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    
    
    MMK_CreateControl(&create );    

    GUILIST_SetItemLayer(ctrl_id, &dev_info);
    MMK_SetCtrlLcdDevInfo(ctrl_id, &dev_info);
     
    #if defined(MMIDC_F_PDA_BIT_16)
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_PRGBAR, FALSE);
    
    GUILIST_SetBgColor(ctrl_id, MMIDC_PDA_LIST_BG_COLOR);
    #endif
}

/*****************************************************************************/
//Description : create camera button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateCameraBtn(MMI_CTRL_ID_T ctrl_id, //in
                                 GUI_BOTH_RECT_T *rect_ptr, //in
                                 uint32 img_id //in
                                 )
{
    MMI_CONTROL_CREATE_T        create = {0};        
    GUIBUTTON_INIT_DATA_T    button_init = {0};
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMK_DestroyControl(ctrl_id);
    
    
    button_init.both_rect = *rect_ptr;
    
    button_init.bg.bg_type = GUI_BG_IMG;
    button_init.bg.img_id  = img_id;
    
    //create del bar
    create.ctrl_id           = ctrl_id;
    create.guid              = SPRD_GUI_BUTTON_ID;
    create.init_data_ptr     = &button_init;
    create.parent_win_handle = win_id;
    MMK_CreateControl(&create);
    
    MMK_SetCtrlLcdDevInfo(ctrl_id, &dev_info);
    
    GUIBUTTON_SetRunSheen(ctrl_id, FALSE);     
}


/*****************************************************************************/
//Description :  init gui desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitGUIDesktopPDA(MMIDC_DESKTOP_PDA_T *pda_ptr  //in
                             )
{
    if (PNULL != pda_ptr)
    {
        SCI_MEMSET(pda_ptr, 0x00, sizeof(*pda_ptr));
        pda_ptr->is_visible = FALSE;
    }
}

/*****************************************************************************/
//Description :  init zoom seekbar param
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitZoomSeekBarParam(void)
{
    SCI_MEMSET(&s_zoom_seekbar_info, 0x00, sizeof(s_zoom_seekbar_info));
}

/*****************************************************************************/
//Description :  init all pda param
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitAllPDAParam(void)
{
    InitGUIDesktopPDA(s_desktop_pda_ptr);
    InitZoomSeekBarParam();
    SCI_MEMSET(&s_pda_icon_rects, 0x00, sizeof(s_pda_icon_rects));
    
    s_menu_key_state = 0;
#if defined(MMIDC_F_U_CAMERA) 
    if(MMIDC_GetUCameraModuleFlag())
    {
        s_button_list_focus = BUTTON_LIST_FOCUS_SETTING;
    }
    else
#endif
    {
        s_button_list_focus = BUTTON_LIST_FOCUS_MODE;
    }
    
    MMIDC_InitPDAKeyStatusParameter();
}
/*****************************************************************************/
//Description : create desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDesktopPDA(void)
{
    BOOLEAN ret = FALSE;
    
    if (PNULL == s_desktop_pda_ptr)
    {
        s_desktop_pda_ptr = SCI_ALLOCA(sizeof(MMIDC_DESKTOP_PDA_T));
        if (PNULL != s_desktop_pda_ptr)
        {
            ret = TRUE;          
            
            InitAllPDAParam();
            CreatePDAAllItems();
        }
    }
    else
    {
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] CreateDesktopPDA ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_1658_112_2_18_2_8_56_252,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//Description : destroy desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void DestroyDesktopPDA(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    uint32 i = 0;
    
    for (i = MMIDC_PDA_CTRL_ID_START; i < MMIDC_PDA_CTRL_ID_END; i++)
    {
        if (PNULL != MMK_GetCtrlHandleByWin(win_id, i))
        {
            MMK_DestroyControl(i);
        }
    }    
    
    DestroyImageAttrInfo();
    
    if (PNULL != s_desktop_pda_ptr)
    {
        SCI_FREE(s_desktop_pda_ptr);
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyDesktopPDA"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_1693_112_2_18_2_8_56_253,(uint8*)"");
}

/*****************************************************************************/
//Description : set desktop visible or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetDesktopVisible(BOOLEAN is_visible)
{
    if (PNULL != s_desktop_pda_ptr)
    {
        s_desktop_pda_ptr->is_visible = is_visible;
    }
}
/*****************************************************************************/
//Description :  reset icon type list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetIconTypeList(MMI_CTRL_ID_T ctrl_id)
{
#if defined(MMIDC_F_PDA_BIT_16)
    GUILIST_SetBgColor(ctrl_id, MMIDC_PDA_LIST_BG_COLOR);
#else
    GUILIST_SetBgImage(ctrl_id, IMG_ICONLIST_TEXT_BG, FALSE);
#endif
    
    GUILIST_SetTextFont(ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
}

/*****************************************************************************/
//Description :  open pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayListCtrl(MMI_CTRL_ID_T ctrl_id, MMIDC_PDA_ICON_E icon_id)
{
    MMIDC_LIST_CTRL_ITEM_T *item_ptr = PNULL;
    BOOLEAN is_ctrl_item = TRUE; 
    
    item_ptr = GetListCtrlItemByPtr();
    
    if (PNULL != item_ptr)
    {
        item_ptr->icon_id = icon_id;
        
        switch(icon_id)
        {
        case PDA_ICON_MODE:
            MMIDC_GetDCModeListItems(item_ptr);
            break;
        case PDA_ICON_IMAGE_ATTR:
            is_ctrl_item = FALSE;
            break;
        case PDA_ICON_SETTING:
            item_ptr->cur_item = GUILIST_GetCurItemIndex(ctrl_id) - 1;
            MMIDC_GetDCSettingListItems(item_ptr);        
            break;
            
        case PDA_ICON_WHITE_BALANCE:
            MMIDC_GetDCWhiteBlanceListItems(item_ptr);        
            break;
            
        case PDA_ICON_EFFECT:
            MMIDC_GetDCEffectListItems(item_ptr);        
            break;

        case PDA_ICON_SETTING_DC_FLASH:
        case PDA_ICON_SETTING_DC_QUALITY:
        case PDA_ICON_SETTING_DC_SIZE:
        case PDA_ICON_SETTING_DC_PIC_MODE:
        case PDA_ICON_SETTING_DC_SELF_SHOT:
        case PDA_ICON_SETTING_DC_CORUSCATE:
        case PDA_ICON_SETTING_DC_STORAGE:        
            MMIDC_GetDCSettingTypeListItems(item_ptr);        
            break;

        case PDA_ICON_SETTING_DV_FLASH:
        case PDA_ICON_SETTING_DV_SIZE:
        case PDA_ICON_SETTING_DV_CORUSCATE:
        case PDA_ICON_SETTING_DV_VIDEOFORMAT:
        case PDA_ICON_SETTING_DV_STORAGE:        
            MMIDC_GetDVSettingTypeListItems(item_ptr);        
            break;    
        default:       
            is_ctrl_item = FALSE;
            break;
        }
        
        if (is_ctrl_item)
        {
            SetListCtrlItem(item_ptr);
            AddListCtrlItem(ctrl_id,
                item_ptr
                );
        }
        
    }
   ResetIconTypeList(ctrl_id);
    
    //SCI_TRACE_LOW:"[MMIDC] DisplayListCtrl is_ctrl_item=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_1790_112_2_18_2_8_56_254,(uint8*)"d",is_ctrl_item);
}

/*****************************************************************************/
//Description :  open pda desktop
//Global resource dependence : none
//Author: kun.liu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateListCtrl(MMI_CTRL_ID_T ctrl_id, MMIDC_PDA_ICON_E icon_id)
{
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    GUILIST_TYPE_E data_type = 0;
    
    MMK_DestroyControl(MMIDC_PDA_LABEL_CTRL_ID);
    MMK_DestroyControl(ctrl_id);


    if(PDA_ICON_SETTING == icon_id)
    {
        data_type = GUILIST_TEXTLIST_E;
    }
    else
    {
  
        data_type = GUILIST_RADIOLIST_E;
    }   
    CreateListCtrl(ctrl_id, data_type);
    
    list_item.icon_id = icon_id;
    
    SetListCtrlItem(&list_item);
}
/*****************************************************************************/
//Description : MoveButtonList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveButtonList(int16 move_len, BOOLEAN is_show)
{
    GUI_RECT_T rect = {0};  
    MMI_CTRL_ID_T i = 0;
    MMIDC_ITEM_CONTENT_T content = {0};
    
    //button bg
    content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
    content.item_data.index = PDA_RECT_BTN_LIST_BG;
    rect = GetMovedRectAndChangeRect(content, move_len);

    if (is_show)
    {
    #if defined(MMIDC_F_PDA_BIT_16)
        {
            GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
        
            LCD_FillRect(&dev_info, rect, MMIDC_PDA_LIST_BG_COLOR);
        }
    #else
        {
            MMIDC_DisplayBmp(rect.left, rect.top,IMG_ICONLIST_BG);
        }
    #endif
    }
    
    //button item
    content.type = MMIDC_ITEM_TYPE_CTRL;

    for (i = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID; i <= MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID; i++)
    {
        content.item_data.ctrl_id = i;
        rect = GetMovedRectAndChangeRect(content, move_len);

        if (is_show)
        {
            MMK_SendMsg(i, MSG_CTL_PAINT, PNULL);
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//Description : Move type list canvas
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveTypeListCanvas(int16 move_len,
                                        MMI_CTRL_ID_T ctrl_id,
                                        BOOLEAN is_show
                                        )
{
#if !defined(MMIDC_F_PDA_BIT_16)
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_LCD_DEV_INFO    dev_info_ptr = MMIDC_GetOSDBlockInfo();    
#endif    
    GUI_RECT_T rect = {0};  
    MMIDC_ITEM_CONTENT_T content = {0};
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    BUTTON_LIST_FOCUS_E focus_button = GetButtonListFocus();
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T brightness_img_id = IMG_BRIGHTNESS_ICON;
    MMI_IMAGE_ID_T contrast_img_id = IMG_CONTRAST_ICON;
    MMI_IMAGE_ID_T ec_img_id = IMG_EC_ICON;
#if !defined(MMIDC_F_PDA_BIT_16)
    MMI_IMAGE_ID_T pic_bg_img_id = IMG_ICONLIST_TEXT_BG;
#endif
    MMI_IMAGE_ID_T seebar_bg_img_id = 0;

    MMI_CTRL_ID_T label_ctrl_id = MMIDC_PDA_LABEL_CTRL_ID;

    if (BUTTON_LIST_FOCUS_IMAGE_PROPERTY == focus_button )
    {
        //list rect bg
        content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
        content.item_data.index = PDA_RECT_LIST_RECT_BG;
        rect = GetMovedRectAndChangeRect(content, move_len);

        if (is_show)
        {
            #if defined(MMIDC_F_PDA_BIT_16)
                MMIDC_ClearRect(rect, MMIDC_PDA_LIST_BG_COLOR);
            #else
                GUIRES_DisplayImg(PNULL,
                                  &rect,
                                  PNULL,
                                  win_id,
                                  pic_bg_img_id,
                                  &dev_info_ptr
                                  );
            #endif
        }
        //title label
        content.type = MMIDC_ITEM_TYPE_CTRL;
        content.item_data.ctrl_id = label_ctrl_id;
        rect = GetMovedRectAndChangeRect(content, move_len);    
        if (is_show)
        {
            MMK_SendMsg(label_ctrl_id, MSG_CTL_PAINT, PNULL);
        }
                          
        //brightness icon
        content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
        if (CAMERA_MODE_DC == mode)
        {
            content.item_data.index = PDA_RECT_PIC_PROPERTY_BRIGHTNESS_ICON;
        }
        else
        {
            content.item_data.index = PDA_RECT_PIC_PROPERTY_DV_BRIGHTNESS_ICON;
        }
        rect = GetMovedRectAndChangeRect(content, move_len);
        x = rect.left;
        y = rect.top;
        if (is_show)
        {
            MMIDC_DisplayBmp(x, y,brightness_img_id);
        }

        //contrast icon
        content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
        if (CAMERA_MODE_DC == mode)
        {
            content.item_data.index = PDA_RECT_PIC_PROPERTY_CONTRAST_ICON;
        }
        else
        {
            content.item_data.index = PDA_RECT_PIC_PROPERTY_DV_CONTRAST_ICON;
        }
        rect = GetMovedRectAndChangeRect(content, move_len);
        x = rect.left;
        y = rect.top;
        if (is_show)
        {        
            MMIDC_DisplayBmp(x, y,contrast_img_id);
        }
        
        //ec icon
        if(CAMERA_MODE_DC == mode)
        {
            content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
            content.item_data.index = PDA_RECT_PIC_PROPERTY_EC_ICON;
            rect = GetMovedRectAndChangeRect(content, move_len);
            x = rect.left;
            y = rect.top;
            
            if (is_show)
            {
                MMIDC_DisplayBmp(x, y,ec_img_id);
            }
        }

        //pic property seekbar bg
        content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
        content.item_data.index = PDA_RECT_PIC_PROPERTY_SEEKBAR_BG;
        if(CAMERA_MODE_DC == mode)
        {
            seebar_bg_img_id = IMG_PIC_PROPERTY_BG;
        }
        else
        {
            seebar_bg_img_id = IMG_DV_PIC_PROPERTY_BG;
        }
        rect = GetMovedRectAndChangeRect(content, move_len);
        
        x = rect.left;
        y  = rect.top; 
        if (is_show)
        {        
            MMIDC_DisplayBmp(x, y, seebar_bg_img_id);
        }
        
        SetImageAttrAllSeekBar(move_len, is_show);
    }
    else
    {
        //list ctrl
        content.type = MMIDC_ITEM_TYPE_CTRL;
        content.item_data.ctrl_id = ctrl_id;
        rect = GetMovedRectAndChangeRect(content, move_len);
        if (is_show)
        {
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//Description : IconListProcessExpandBtn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IconListProcessExpandBtn(MMI_HANDLE_T ctrl_id, BOOLEAN is_press)
{
    GUI_RECT_T rect = {0};    
    int16 move_len  = 0;      
    int16 slided_len = 0;
    int16 total_len = 0;
    int16 move_unit = 0;
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    MMI_CTRL_ID_T list_ctrl_id = 0;
    MMIDC_ITEM_CONTENT_T content = {0};
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    BOOLEAN is_show = TRUE;
    MMI_IMAGE_ID_T img_id = 0;
#ifdef MMIDC_F_N_IN_ONE
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
#endif
    
    GetListCtrlItem(&list_item);
    switch(list_item.icon_id)
    {
        case PDA_ICON_SETTING_DC_FLASH:
        case PDA_ICON_SETTING_DC_QUALITY:
        case PDA_ICON_SETTING_DC_SIZE:
        case PDA_ICON_SETTING_DC_PIC_MODE:
        case PDA_ICON_SETTING_DC_SELF_SHOT:   
        case PDA_ICON_SETTING_DC_CORUSCATE:   
        case PDA_ICON_SETTING_DC_STORAGE:
        case PDA_ICON_SETTING_DV_FLASH:
        case PDA_ICON_SETTING_DV_SIZE:   
        case PDA_ICON_SETTING_DV_CORUSCATE: 
        case PDA_ICON_SETTING_DV_VIDEOFORMAT:
        case PDA_ICON_SETTING_DV_STORAGE:
            list_ctrl_id = MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID;
        break;
        default:
            list_ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
        break;
    }

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        total_len = MOVE_HEIGHT_V;
        move_unit = EXPAND_MOVE_LEN_V;
    }
    else
    {
        total_len = MOVE_WIDTH;
        move_unit = EXPAND_MOVE_LEN;
    }
    
    move_len = move_unit;
    if (is_press)
    {    
        move_len = -move_len;
    }
    
    do
    {    
        ClearExpandPanelRect();
        
        if( (total_len - slided_len) < move_unit)
        {
            move_unit = total_len - slided_len;
            move_len = move_unit;
            if (is_press)
            {
                move_len = -move_len;
            }            
#ifdef MMIDC_F_N_IN_ONE
    if(MMIDC_IsNInOneEnable() && (DC_PREVIEW_MODE == mode) && is_press)
    {
        MMIDC_DisplayNInOne();
    }
#endif

            //change expand btn picture
            ResetExpandBtn();
        }


        //mode button list
        MoveButtonList(move_len, is_show);

        //list canvas
        MoveTypeListCanvas(move_len, list_ctrl_id, is_show);
        
        //list shadow
        content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
        content.item_data.index = PDA_RECT_LIST_SHADOW;
        rect = GetMovedRectAndChangeRect(content, move_len);
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            img_id = IMG_ICONLIST_SHADOW_V;
        }
        else
        {
            img_id = IMG_ICONLIST_SHADOW;
        }
        if (is_show)
        {
            MMIDC_DisplayBmp(rect.left, rect.top,img_id);
        }

        //expand ctrl
        content.type = MMIDC_ITEM_TYPE_CTRL;
        content.item_data.ctrl_id = ctrl_id;
        rect = GetMovedRectAndChangeRect(content, move_len);
        if (is_show)
        {
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }

        
        MMIDC_CompleteDisplayOSD();
        

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            SCI_Sleep(MMIDC_ANIM_VERTICAL_SLEEP_TIME);
        }
        
        slided_len += move_unit;
    }while(slided_len < total_len);
    
    return TRUE;
}
/*****************************************************************************/
//Description : SetImageAttrAllSeekBar
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN SetImageAttrAllSeekBar(int16 move_len, BOOLEAN is_show)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();    
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    uint16 i = 0;
    uint16 total = 0;
    int16 delta_move_len = 0;

    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    if (PNULL != attr_ptr)
    {
        delta_move_len = move_len;

        total = attr_ptr->total_num;
        for (i = 0; i < total; i++)
        {
            //get seekbar info
            seekbar_info = attr_ptr->attr[i];

            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                seekbar_info.frame.cur_rect.top += delta_move_len;                
                seekbar_info.frame.cur_rect.bottom += delta_move_len;                
                
                seekbar_info.bg.cur_rect.top += delta_move_len;                
                seekbar_info.bg.cur_rect.bottom += delta_move_len;
                
                seekbar_info.mask.cur_rect.top += delta_move_len;                
                seekbar_info.mask.cur_rect.bottom += delta_move_len;
                
                seekbar_info.scale.cur_rect.top += delta_move_len;                
                seekbar_info.scale.cur_rect.bottom += delta_move_len;
            }
            else
            {
                seekbar_info.frame.cur_rect.left += delta_move_len;                
                seekbar_info.frame.cur_rect.right+= delta_move_len;                
                
                seekbar_info.bg.cur_rect.left += delta_move_len;                
                seekbar_info.bg.cur_rect.right+= delta_move_len;
                
                seekbar_info.mask.cur_rect.left += delta_move_len;                
                seekbar_info.mask.cur_rect.right += delta_move_len;
                
                seekbar_info.scale.cur_rect.left += delta_move_len;                
                seekbar_info.scale.cur_rect.right += delta_move_len;
            }

            if (is_show)
            {
                DrawSeekBar(&seekbar_info); 
            }
            
            //set seekbar info
            attr_ptr->attr[i] = seekbar_info;

        }
    }
    return TRUE;
}

/*****************************************************************************/
//Description : process expand button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessExpandBtn(MMI_HANDLE_T ctrl_id
                                )
{
    BOOLEAN ret   = FALSE;
    BOOLEAN is_press = FALSE;
    BOOLEAN is_expand_state = FALSE;
    MMIDC_LIST_CTRL_ITEM_T *item_ptr = PNULL;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();

    BUTTON_LIST_FOCUS_E btn_list_focus = 0;
    BOOLEAN is_attr_press = FALSE;
    MMIDC_LIST_CTRL_ITEM_T item = {0};
    
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    is_expand_state = !is_press;
    item_ptr = GetListCtrlItemByPtr();
    if(item_ptr != PNULL)
    {        
        SetExpandPanelVisible(TRUE);  
        SetIconStateAsPress(PDA_RECT_EXPAND, is_expand_state);

        //update current list ctrl content
        if (is_expand_state)
        {
            GetListCtrlItem(&item);
            DisplayListCtrl(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, item.icon_id);
        }        

        if(is_press)
        {
            IconListProcessExpandBtn(ctrl_id, is_press);
        }
        ResetCaptureBtn(mode);
        ResetSensorBtn();
#if defined (MMIDC_F_U_CAMERA)
        ResetUCameraBtn();
#endif
        ResetPicBtn();
        MMK_SendMsg(MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID, MSG_CTL_PAINT, PNULL);
        MMK_SendMsg(MMIDC_PDA_BUTTON_SENSOR_CTRL_ID, MSG_CTL_PAINT, PNULL);
#ifdef PIC_VIEWER_SUPPORT
        MMK_SendMsg(MMIDC_PDA_BUTTON_PIC_CTRL_ID, MSG_CTL_PAINT, PNULL);
#endif
#if defined (MMIDC_F_U_CAMERA)
        MMK_SendMsg(MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID, MSG_CTL_PAINT, PNULL);
#endif
        if(!is_press)
        {
            IconListProcessExpandBtn(ctrl_id, is_press);
        }

        MMIDC_CompleteDisplayOSD();

        //check image property state for seekbar
         is_expand_state = GetIconStateAsPress(PDA_RECT_EXPAND);
        if (is_expand_state)
        {
             btn_list_focus = GetButtonListFocus();
            if (BUTTON_LIST_FOCUS_IMAGE_PROPERTY == btn_list_focus)
            {
                is_attr_press = TRUE;
            }
        }
        else
        {
            is_attr_press = FALSE;
        }
        SetIconStateAsPress(PDA_RECT_IMAGE_PROPERTY, is_attr_press);

    }

    //SCI_TRACE_LOW:"[MMIDC] ProcessExpandBtn is_expand_state=%d, btn_list_focus=%d, is_attr_press=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2278_112_2_18_2_8_57_255,(uint8*)"ddd",is_expand_state,btn_list_focus,is_attr_press);

    return ret;
}

/*****************************************************************************/
//Description : process cmd1 button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCmd1Btn(void)
{
    BOOLEAN ret = TRUE;    
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMK_SendMsg(win_id, MSG_APP_OK, PNULL);    

   //SCI_TRACE_LOW:"[MMIDC] ProcessCmd1Btn"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2294_112_2_18_2_8_57_256,(uint8*)"");
        
    return ret;
}

/*****************************************************************************/
//Description : process cmd2 button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCmd2Btn(void)
{
    BOOLEAN ret = TRUE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);    
   
    //SCI_TRACE_LOW:"[MMIDC] ProcessCmd2Btn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2310_112_2_18_2_8_57_257,(uint8*)"");
        
    return ret;
}

/*****************************************************************************/
//Description : process cmd3 button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCmd3Btn(void)
{
    BOOLEAN ret = TRUE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMK_SendMsg(win_id, MSG_DC_RETURN_KEY, PNULL);    

   //SCI_TRACE_LOW:"[MMIDC] ProcessCmd3Btn"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2326_112_2_18_2_8_57_258,(uint8*)"");
    
    return ret;
}

/*****************************************************************************/
//Description : process capture button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCaptureBtn(void)
{

    BOOLEAN ret = FALSE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    
    if(!MMIDC_IsReviewing() && !MMIDC_IsCapturing())
    {
        MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);

        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessCaptureBtn ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2349_112_2_18_2_8_57_259,(uint8*)"d",ret);
             
    return ret;
}

/*****************************************************************************/
//Description : process capture key
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCaptureKey(void)
{
    BOOLEAN ret = FALSE;

    if (!MMIDC_IsReviewing() && !MMIDC_IsCapturing())
    {
        MMIDC_InitParamCtrl();
        SetIconStateAsPress(PDA_RECT_ZOOM, FALSE);

        SetExpandPanelVisible(FALSE);  
        SetCaptureBtnVisible(FALSE);    
        SetSensorBtnVisible(FALSE);    
        SetZoomBtnVisible(FALSE);    
#if defined(MMIDC_F_U_CAMERA)        
        SetUCameraBtnVisible(FALSE);
#endif
        SetPicBtnVisible(FALSE);
#ifdef MMIDC_F_LAST_IMAGE
        ClearLastThumbnailRect();
#endif

        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessCaptureKey ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2381_112_2_18_2_8_57_260,(uint8*)"d",ret);
             
    return ret;
}
/*****************************************************************************/
//Description : process list ctrl 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCProcessListCtrl(MMI_HANDLE_T ctrl_id
                                    )
{
    BOOLEAN ret = FALSE;
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    uint16 cur_item = 0;
    uint16 max_item_num = 0;
    PDA_ITEM_CALL_BACK call_back_ptr = PNULL;
    
    GetListCtrlItem(&list_item);
    max_item_num = list_item.max_item_num;
    
    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
    if ((cur_item - 1) < max_item_num)
    {
        call_back_ptr = list_item.item_callback[cur_item - 1];
        
        if (PNULL != call_back_ptr)
        {
            call_back_ptr();
        }
    }
    
    return ret;
}


/*****************************************************************************/
//Description : process Buttonlist ctrl 
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessButtonList(MMI_CTRL_ID_T ctrl_id
                              )
{
    BOOLEAN ret = TRUE;
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    BUTTON_LIST_FOCUS_E focus_btn = 0;
    BOOLEAN is_list_ctrl_visible = TRUE;
    
    SetIconStateAsPress(PDA_RECT_IMAGE_PROPERTY, FALSE);

   
    switch(ctrl_id)
    {
    case MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID:
        list_item.icon_id = PDA_ICON_MODE;    
        focus_btn =BUTTON_LIST_FOCUS_MODE;
        break;
        
    case MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID:
        {
            MMIDC_LIST_CTRL_ITEM_T *item_ptr = PNULL;
            
            item_ptr = GetListCtrlItemByPtr();
            if (PNULL != item_ptr)
            {
                item_ptr->icon_id = PDA_ICON_IMAGE_ATTR;
                
                InitImageAttrInfo();
                
                SetIconStateAsPress(PDA_RECT_IMAGE_PROPERTY, TRUE);
            }
            
            list_item.icon_id = PDA_ICON_IMAGE_ATTR;
            focus_btn = BUTTON_LIST_FOCUS_IMAGE_PROPERTY;
            
            is_list_ctrl_visible = FALSE;
        }
        break;
        
    case MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID:
        list_item.icon_id = PDA_ICON_WHITE_BALANCE;    
        focus_btn = BUTTON_LIST_FOCUS_WHITEBLANCE;
        break;
        
    case MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID:
        list_item.icon_id = PDA_ICON_EFFECT;    
        focus_btn = BUTTON_LIST_FOCUS_EFFECT;
        break;
        
    case MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID:
        list_item.icon_id = PDA_ICON_SETTING; 
        focus_btn = BUTTON_LIST_FOCUS_SETTING;
        break;
        
    default:
        break;
    }    

    SetListCtrlItem(&list_item);
    SetButtonListFocus(focus_btn);


    ClearExpandPanelRect();
    ResetButtonList();

    DestroyIconTypeListCtrl();
    
    if (PDA_ICON_IMAGE_ATTR != list_item.icon_id)
    {
        MMIDC_CreateListCtrl(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, list_item.icon_id);
    }

    //SCI_TRACE_LOW:"[MMIDC] ProcessButtonList ctrl_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2493_112_2_18_2_8_57_261,(uint8*)"d",ctrl_id);
    return ret;
}

/*****************************************************************************/
//Description :  MMIDC_SetDCIconTypeList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_SetDCIconTypeList(MMI_HANDLE_T ctrl_id) 
{
    BOOLEAN     ret = TRUE;
	BOOLEAN		is_clear = TRUE;
    uint16      cur_index = 0;
    MMIDC_LIST_CTRL_ITEM_T list_item = {0}; 
    
    GetListCtrlItem(&list_item);
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);   
    cur_index -= 1; //index from 1
    switch(list_item.icon_id)
    {
        case PDA_ICON_MODE:
        {
            ResetCaptureBtn((CAMERA_MODE_E)cur_index);
#if defined(MMIDC_F_U_CAMERA)            
            ResetUCameraBtn();
#endif            
            
            MMIDCProcessListCtrl(ctrl_id);
            ResetPicBtn();
            ResetZoomBtn();
            ResetButtonList();
        }
        break;
        
        case PDA_ICON_WHITE_BALANCE:
        {
            MMIDCProcessListCtrl(ctrl_id);            
        }
        break;
        
        case PDA_ICON_EFFECT:
        {
            MMIDCProcessListCtrl(ctrl_id); 
			is_clear = FALSE;
        }
        break;
        
        case PDA_ICON_SETTING:
        {
            ret = MMIDC_SetDCIconSettingList(ctrl_id);
        }
        break;
        
        default:
        break;
    }

	if(is_clear)
	{
		ClearExpandPanelRect();
		ret = TRUE;
	}
    else
    {
        ret = FALSE;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetDCIconTypeList icon_id=%d, cur_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2553_112_2_18_2_8_57_262,(uint8*)"dd",list_item.icon_id, cur_index);
    return ret;
}

/*****************************************************************************/
//Description :  MMIDC_SetDCIconSettingTypeList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_SetDCIconSettingTypeList(MMI_HANDLE_T ctrl_id) 
{
    BOOLEAN ret = TRUE;
    uint16      cur_index = 0;
    MMIDC_LIST_CTRL_ITEM_T list_item = {0}; 
    CAMERA_MODE_E   mode = MMIDC_GetCameraMode(); 
    int8    max_device = 0;
    MMIFILE_DEVICE_E file_dev = 0;
    MMIFILE_DEVICE_E  dev_list[MMI_DEVICE_NUM] = {0};
    max_device = GetStorageDeviceList(dev_list, PNULL);
    
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);   
    GetListCtrlItem(&list_item);
    
    if (CAMERA_MODE_DC == mode)
    {
        switch(list_item.icon_id)
        {
#ifdef MMIDC_F_FLASH
        case PDA_ICON_SETTING_DC_FLASH:
            {
            MMIDC_SetFlashLamp((DCAMERA_FLASH_MODE_E)(cur_index - 1));
            }
            break;
#endif

        case PDA_ICON_SETTING_DC_QUALITY:
            {
            MMIDC_SetPhotoQuality(cur_index - 1);
            }
            break;
            
        case PDA_ICON_SETTING_DC_SIZE:
            {
                MMIDCProcessListCtrl(ctrl_id);
            }
            break;
            
        case PDA_ICON_SETTING_DC_PIC_MODE:
            {
            MMIDC_SetMultiShootEnum(cur_index - 1);
            }
            break;
            
        case PDA_ICON_SETTING_DC_SELF_SHOT:
            {
                MMIDC_SetSelfShootDelayTime(cur_index - 1);
            }
            break;
            
        case PDA_ICON_SETTING_DC_CORUSCATE:
            {
                MMIDC_SetPhotoFlicker(cur_index - 1);
            }
            break;
            
        case PDA_ICON_SETTING_DC_STORAGE:
            {
                if(max_device > 0)
                {
                    
                    file_dev = dev_list[cur_index - 1];

                    if(file_dev != MMIDC_GetPrefPhotoStorageDevice())
                    {                        
                        MMIDC_PostCreateFileHandleMsg();
                    }
                                            
                    MMIDC_SetPrefPhotoStorageDevice(file_dev);
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch(list_item.icon_id)
        {
#ifdef MMIDC_F_FLASH
        case PDA_ICON_SETTING_DV_FLASH:
            {
            MMIDC_SetFlashLamp((DCAMERA_FLASH_MODE_E)(cur_index - 1));
            }
            break;
#endif
            
        case PDA_ICON_SETTING_DV_SIZE:
            {
                MMIDCProcessListCtrl(ctrl_id);
            }
            break;
            
        case PDA_ICON_SETTING_DV_CORUSCATE:
            {
                MMIDC_SetVideoFlicker((FLICKER_E)(cur_index - 1));
            }
            break;
            
        case PDA_ICON_SETTING_DV_VIDEOFORMAT:
            {
                MMIDC_SetRecordFileTypeOfSetting((VIDEO_FILE_TYPE_E)(cur_index - 1));
            }
            break;
            
        case PDA_ICON_SETTING_DV_STORAGE:
            {
                if(max_device > 0)
                {
                    file_dev = dev_list[cur_index - 1];
                    MMIDC_SetPrefVideoStorageDevice(file_dev);
                }
            }
            break;
        default:
            break;
        }
    }

    
    ClearExpandPanelRect();
    MMK_DestroyControl(ctrl_id);
    MMIDC_CreateListCtrl(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING);
    MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID);
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetDCIconSettingTypeList ctrl_id=%d,cur_index=%d,icon_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2674_112_2_18_2_8_58_263,(uint8*)"ddd",ctrl_id, cur_index, list_item.icon_id);
    
    return ret;
}

/*****************************************************************************/
//Description : process pic button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessPicBtn(void)
{
    BOOLEAN ret = FALSE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();


    SetIconStateAsPress(PDA_RECT_ZOOM, FALSE);

    if (CAMERA_MODE_DC == mode)
    {
        MMIDC_OpenPictureWin();
    }
    else
    {
        MMIDC_OpenMovieWin();
    }

    //SCI_TRACE_LOW:"[MMIDC] ProcessPicBtn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2700_112_2_18_2_8_58_264,(uint8*)"");
    
    return ret;
}

/*****************************************************************************/
//Description : process sensor button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessSensorBtn(void)
{
    BOOLEAN ret = FALSE;

    SetIconStateAsPress(PDA_RECT_ZOOM, FALSE);
    
    MMIDC_ShortcutForSwitchSensor();

    ResetImageAttrInfo();

    //SCI_TRACE_LOW:"[MMIDC] ProcessSensorBtn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2719_112_2_18_2_8_58_265,(uint8*)"");
    
    return ret;
}

/*****************************************************************************/
//Description : process zoom btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessZoomBtn(void)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_press = FALSE;
    BOOLEAN is_hor_draw = FALSE;
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    is_press = GetIconStateAsPress(PDA_RECT_ZOOM);
    is_press = !is_press;            
    if (is_press)
    {   

        HideExpandPanelBtn(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);   
        SetExpandPanelVisible(FALSE);        

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            is_hor_draw = TRUE;
        }
        InitZoomSeekBarInfo(is_hor_draw);
    }
    else
    {
        SetExpandPanelVisible(TRUE);  
    }
    
    SetIconStateAsPress(PDA_RECT_ZOOM, is_press);
    ResetZoomBtn();

    ClearExpandPanelRect();

    //SCI_TRACE_LOW:"[MMIDC] ProcessZoomBtn ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2760_112_2_18_2_8_58_266,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//Description : process arrow left btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessArrowLeftBtn(void)
{
    BOOLEAN ret = FALSE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID;

    BOOLEAN is_visible = FALSE;

    is_visible = IsCtrlVisible(ctrl_id);
    if(is_visible)
    {
        MMK_SendMsg(win_id, MSG_APP_LEFT, PNULL);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessArrowLeftBtn ret=%d, is_visible=%D"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2784_112_2_18_2_8_58_267,(uint8*)"dd",ret, is_visible);
             
    return ret;
}

/*****************************************************************************/
//Description : process arrow right btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessArrowRightBtn(void)
{
    BOOLEAN ret = FALSE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID;

    BOOLEAN is_visible = FALSE;

    is_visible = IsCtrlVisible(ctrl_id);
    if(is_visible)
    {
        MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessArrowRightBtn ret=%d, is_visible=%D"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2808_112_2_18_2_8_58_268,(uint8*)"dd",ret, is_visible);
             
    return ret;
}

#if defined (MMIDC_F_U_CAMERA)
/*****************************************************************************/
//Description : process u camera button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessUCameraBtn(void)
{
    BOOLEAN is_press = FALSE;
    
    
    is_press = GetIconStateAsPress(PDA_RECT_ZOOM);

    //SCI_TRACE_LOW:"[MMIDC] ProcessUCameraBtn is_press=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2826_112_2_18_2_8_58_269,(uint8*)"d",is_press);
    
    if(is_press)
    {
        ProcessZoomBtn();
    }
    MMIDC_OpenUCameraWin();
    return TRUE;
}
#endif

/*****************************************************************************/
//Description : process msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTPPenOK(MMI_HANDLE_T ctrl_id
                            )
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_update = TRUE;
    
    switch(ctrl_id)
    {
    case MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID:
        is_update = FALSE;
        ret = ProcessCaptureBtn();
        break;
    case MMIDC_PDA_BUTTON_EXPAND_CTRL_ID:
        is_update = FALSE;
        ret = ProcessExpandBtn(ctrl_id);
        break;
#if defined (MMIDC_F_U_CAMERA)      
    case MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID:
        is_update = FALSE;
        ret = ProcessUCameraBtn();
        break;
#endif      
    case MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID:
    case MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID:
    case MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID:
    case MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID:
    case MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID:
        ret = ProcessButtonList(ctrl_id);
        break;   
        
    case MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID:
        ret = MMIDC_SetDCIconTypeList(ctrl_id);
        if(FALSE == ret)
        {
            is_update = FALSE;
        }
        break;
        
    case MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID:
        ret = MMIDC_SetDCIconSettingTypeList(ctrl_id);
        break;
    case MMIDC_PDA_BUTTON_ZOOM_CTRL_ID:
        ret = ProcessZoomBtn();
        break;
    case MMIDC_PDA_BUTTON_CMD_1_CTRL_ID:
        is_update = FALSE;
        ret = ProcessCmd1Btn();
        break;
    case MMIDC_PDA_BUTTON_CMD_2_CTRL_ID:
        is_update = FALSE;
        ret = ProcessCmd2Btn();
        break;
    case MMIDC_PDA_BUTTON_CMD_3_CTRL_ID:
        is_update = FALSE;
        ret = ProcessCmd3Btn();
        break;
		
#ifdef PIC_VIEWER_SUPPORT
    case MMIDC_PDA_BUTTON_PIC_CTRL_ID:
        is_update = FALSE;
        ret = ProcessPicBtn();
        break;
#endif

    case MMIDC_PDA_BUTTON_SENSOR_CTRL_ID:
        is_update = FALSE;
        ret = ProcessSensorBtn();
        break;
    case MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID:
        is_update = FALSE;
        ret = ProcessArrowLeftBtn();
        break;
    case MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID:
        is_update = FALSE;
        ret = ProcessArrowRightBtn();
        break;
#if 0//defined(MMIDC_F_EFFECT_360)        
    case MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID:
        is_update = FALSE;
        ret = ProcessEffect360LeftBtn();
        break;
    case MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID:
        is_update = FALSE;
        ret = ProcessEffect360RightBtn();
        break;        
#endif        
    default:
        is_update = FALSE;
        break;
    }
    
    if (is_update)
    {
        MMIDC_ShowAllOSD();
    }

    //SCI_TRACE_LOW:"[MMIDC] HandleTPPenOK ctrl_id=%d,is_update=%d, ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_2930_112_2_18_2_8_58_270,(uint8*)"ddd",ctrl_id, is_update, ret);
    
    return ret;
}

/*****************************************************************************/
//Description : start osd anim timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartOSDAnimTimer(void)
{
#if 0
    if (0 == s_osd_anim_timer)
    {
        s_osd_anim_timer = MMK_CreateTimer(100, FALSE);
    }
#endif    
}

/*****************************************************************************/
//Description : stop osd anim timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopOSDAnimTimer(void)
{
#if 0
    uint16 timer_id = s_osd_anim_timer;
    
    if (0 != timer_id)
    {
        MMK_StopTimer(timer_id);
        s_osd_anim_timer = 0;
    }
#endif    
}

/*****************************************************************************/
//Description : handle osd anim timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleOSDAnimTimer(uint16 timer_id)
{
    BOOLEAN ret = FALSE;

#if 0
    GUI_RECT_T rect = {0,0,479,319};
    MMI_IMAGE_ID_T img_id = IMG_PREVIEW_ANIM_1 + s_osd_anim_index;
    uint16 timer = s_osd_anim_timer;
    
    if (timer == timer_id)
    {
        ret = TRUE;

        DisplayOSDAnim(rect, img_id);
        StopOSDAnimTimer();

        s_osd_anim_index++;
        if (s_osd_anim_index < 7)
        {
            StartOSDAnimTimer();
        }
    }
#endif

    return ret;
}

/*****************************************************************************/
//Description : Process capture anim before ctrl
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessCaptureAnimBefore(void)
{
    BOOLEAN ret = TRUE;    

     return ret;
}

/*****************************************************************************/
//Description : handle tp move
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpMove(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    do
    {
        ret = HandleZoomSeekBarTpMove(point);
        if (ret)
        {
            break;
        }
        
        ret = HandleImageAttrSeekBarTpMove(point);
        if (ret)
        {
            break;
        }            
    }while(0);

    //SCI_TRACE_LOW:"[MMIDC] HandleTpMove ret=%d, point.x=%d,point.y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3032_112_2_18_2_8_58_271,(uint8*)"ddd",ret,point.x, point.y);
           
    return ret;            
}

/*****************************************************************************/
//Description : is contract for expand panel
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsContract(GUI_POINT_T point)
{
    BOOLEAN is_press = FALSE;
    uint16 i = 0;
    GUI_RECT_T point_rect = {0};
    uint16 screen_width = 0;    
    uint16 screen_height = 0;    
    GUI_RECT_T rect = {0};    
    DC_CONTROL_ID_E ctrl_id_array[] = 
    {
        MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID,   
        MMIDC_PDA_BUTTON_SENSOR_CTRL_ID,
        MMIDC_PDA_BUTTON_ZOOM_CTRL_ID, 
#ifdef PIC_VIEWER_SUPPORT        
        MMIDC_PDA_BUTTON_PIC_CTRL_ID,
#endif        
        MMIDC_PDA_BUTTON_EXPAND_CTRL_ID,  
#if defined(MMIDC_F_U_CAMERA)   
        MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID,
#endif
    };
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    BOOLEAN is_contract = FALSE;
    
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    if(is_press)
    {            
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
        GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &rect);

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            point_rect.left = 0;
            point_rect.right = screen_width - 1;
            point_rect.top = rect.top + 1;        
            point_rect.bottom = screen_height - 1;
        }
        else
        {
            point_rect.left = rect.left + 1;
            point_rect.right = screen_width - 1;
            point_rect.top = 0;        
            point_rect.bottom = screen_height - 1;
        }
        if((point.x >= point_rect.left) 
            && (point.x <= point_rect.right) 
            && (point.y >= point_rect.top) 
            && (point.y <= point_rect.bottom))
        {
            is_contract = TRUE;
            
            for(i = 0; i < ARR_SIZE(ctrl_id_array); i++)
            {                        
                SCI_MEMSET(&rect, 0x00, sizeof(rect));

                GUIAPICTRL_GetRect(ctrl_id_array[i], &rect);
                
                if(IsCtrlVisible(ctrl_id_array[i]) 
                   && GUI_PointIsInRect(point, rect)
                   )
                {
                    is_contract = FALSE;
                    break;
                }
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] IsContract is_press=%d,is_contract=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3108_112_2_18_2_8_59_272,(uint8*)"dd",is_press, is_contract);

    return is_contract;
}

/*****************************************************************************/
//Description : handle contract expand btn tp down
//Global resource dependence : 
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleContractExpandBtnTpDown(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_press = FALSE;
    BOOLEAN is_visible = FALSE;
    GUI_RECT_T rect = {0};

    
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    is_visible = IsCtrlVisible(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
    if(is_press && is_visible)
    {
        /*Since application has response MSG_CTL_BUTTON_DOWN message,
        so don't response the tp down message when click on button*/
        GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &rect);
        if(!GUI_PointIsInRect(point, rect))
        {
            s_pda_expand_info.is_contract = TRUE; 
            s_pda_expand_info.down_point = point;
            ret = TRUE;
        }
    }

    SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpDown is_press=%d",is_press );

    SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpDown is_contrace(%d), (%d,%d)", 
        s_pda_expand_info.is_contract, s_pda_expand_info.down_point.x, s_pda_expand_info.down_point.y);
    
    return ret;
}


/*****************************************************************************/
//Description : handle contract expand btn
//Global resource dependence : 
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleContractExpandBtnTpUp(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_press = FALSE;
    BOOLEAN is_visible = FALSE;
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    uint16 screen_width = 0;    
    uint16 screen_height = 0;   
    GUI_RECT_T rect = {0};
    int16 x_diff = 0;
    int16 y_diff = 0;
    int16 glide_len = 0;


    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    is_visible = IsCtrlVisible(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);

    if(s_pda_expand_info.is_contract && is_press && is_visible)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
        GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &rect);

        /*If down point, up point are both on button, application will 
        response MSG_CTL_PENOK message, so don't response here*/
        
        if(!GUI_PointIsInRect(point, rect) || 
            !GUI_PointIsInRect(s_pda_expand_info.down_point, rect))
        {
            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                rect.left = 0;
                rect.right = screen_width - 1;
            }
            else
            {
                rect.top = 0;
                rect.bottom = screen_height - 1;
            }

    /*
            if(IsContract(s_pda_expand_info.down_point))
            {
                SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp - down_point is in contrct area");
            }
            if(IsContract(point))
            {
                SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp - up_point is in contrct area");
            }
    */
            if ((IsContract(s_pda_expand_info.down_point))&& 
                (IsContract(point)))
            {
                x_diff = s_pda_expand_info.down_point.x - point.x;
                y_diff = s_pda_expand_info.down_point.y - point.y;
                if(((x_diff >= 0 && x_diff <= MMIDC_EXPAND_POINT_DIFF) 
                    || (x_diff<= 0 && x_diff >= -1 * MMIDC_EXPAND_POINT_DIFF))
                    &&((y_diff >= 0 && y_diff <= MMIDC_EXPAND_POINT_DIFF)
                    || (y_diff <= 0 && y_diff >= -1 * MMIDC_EXPAND_POINT_DIFF)))
                {
                    SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp - Click on Contract area.");
                    ProcessExpandBtn(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
                    ret = TRUE;
                }
                SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp - slide on Contract area.");
                
            }
            else if(GUI_PointIsInRect(s_pda_expand_info.down_point, rect))
            {
                if (SCREEN_MODE_VERTICAL == screen_mode)
                {
                    glide_len = point.y - s_pda_expand_info.down_point.y;
                }
                else
                {
                    glide_len = point.x - s_pda_expand_info.down_point.x;
                }
                
                
                if(glide_len <= 0)        
                {
                    ProcessExpandBtn(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
                    ret = TRUE;
                }
                SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp - slide on expand button(%d).", glide_len);
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] HandleContractExpandBtnTpUp is_press=%d, is_contract=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3166_112_2_18_2_8_59_275,(uint8*)"dd",is_press, s_pda_expand_info.is_contract);
               
    SCI_TRACE_LOW("[MMIDC] HandleContractExpandBtnTpUp ret=%d, (%d,%d)",ret, point.x, point.y);
    
    return ret;
}

/*****************************************************************************/
//Description : expend button down
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void HandleExpandButtonDown(void)
{
    BOOLEAN is_visible = FALSE;
    BOOLEAN is_press = FALSE;
    GUI_POINT_T point = {0};
    GUI_RECT_T rect = {0};
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    is_visible = IsCtrlVisible(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if(is_visible)
    {
        GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &rect);
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            point.x = rect.right;
            if(is_press)
            {
                point.y = rect.bottom;
            }
            else
            {
                point.y = rect.top;
            }
        }
        else
        {
            point.x = rect.top;
            if(is_press)
            {
                point.y = rect.right;
            }
            else
            {
                point.y = rect.left;
            }
        }

        s_pda_expand_info.down_point = point;
        
        if(is_press)
        {
            s_pda_expand_info.is_contract = TRUE; 
            
        }
        else
        {
            s_pda_expand_info.is_glide = TRUE;
        }
    }

    SCI_TRACE_LOW("[MMIDC] HandleExpandButtonDown - is visible(%d), is_press(%d),point(%d,%d)",
        is_visible, is_press, point.x, point.y);
}

/*****************************************************************************/
//Description : process msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN DesktopPDAProcessMsg(MMI_MESSAGE_ID_E msg_id, //in
                                   void *param //in
                                   )
{
    BOOLEAN ret = TRUE;
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = 0;
    
    ctrl_id = MMK_GetActiveCtrlId(win_id);
    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENLONGOK:
    case MSG_CTL_PENOK:
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        ret = HandleTPPenOK(ctrl_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_BUTTON_DOWN:
        if(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
        {
            SCI_MEMSET(&s_pda_expand_info, 0x00, sizeof(s_pda_expand_info));
            HandleExpandButtonDown();
        }
        break;
/*
    case MSG_CTL_BUTTON_UP:
        if(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
        {
        }
        break;        
*/
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
            do
            {                  
                SCI_MEMSET(&s_pda_expand_info, 0x00, sizeof(s_pda_expand_info));
                
                ret = HandleZoomSeekBarTpDown(point);
                if (ret)
                {
                    break;
                }
                
                ret = HandleImageAttrSeekBarTpDown(point);
                if (ret)
                {
                    break;
                }

                ret = HandleGlideExpandTpDown(point);
                if (ret)
                {
                    break;
                }

                ret = HandleContractExpandBtnTpDown(point);
                if (ret)
                {
                    break;
                }   
            }while(0);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
            do
            {
                ret = HandleZoomSeekBarTpUp(point);
                if (ret)
                {
                    break;
                }
                
                ret = HandleImageAttrSeekBarTpUp(point);
                if (ret)
                {
                    break;
                }       
                
                ret = HandleGlideExpandTpUp(point);
                if (ret)
                {
                    break;
                }   
                
                ret = HandleContractExpandBtnTpUp(point);
                if (ret)
                {
                    break;
                }   
            }while(0);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:    
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            ret = HandleTpMove(point);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TIMER:
        do
        {
            ret = HandleOSDAnimTimer(*((uint8*)param));
            if (ret)
            {
                break;
            }
#if defined(MMIDC_F_DV_ZOOM)
            ret = HandleZoomSeekBarDisplayTimer(*((uint8*)param));
            if (ret)
            {
                break;
            }
#endif
#ifdef MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION
            ret = HandleZoomSeekBarTimer(*((uint8*)param));
            if (ret)
            {
                break;
            }
            
            ret = HandleImageAttrSeekBarTimer(*((uint8*)param));
            if (ret)
            {
                break;
            }
#endif
        }while(0);
        break;
    
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_CAMERA:
    
    case MSG_APP_OK:
    case MSG_CTL_OK:

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:   

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
    case MSG_DC_RETURN_KEY:
        
    case MSG_APP_MENU:
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
        ret = HandlePDAKey(msg_id, param);
        
        if (ret)
        {
            MMIDC_ShowAllOSD();
        }
        break;
    default:
        ret = FALSE;
        break;
    }
#if defined(MMIDC_F_DV_ZOOM)
    HandleOtherMsgToDisplaySeekBar(ret, msg_id, param);    
#endif
    //SCI_TRACE_LOW:"[MMIDC] DesktopPDAProcessMsg ret=%d, msg_id=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3337_112_2_18_2_8_59_277,(uint8*)"dd",ret, msg_id);
    
    return ret;
}

/*****************************************************************************/
//Description :  get button capture both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnBothRect(GUI_BOTH_RECT_T *both_rect_ptr,//in
                          MMI_IMAGE_ID_T h_img_id, //in
                          MMI_IMAGE_ID_T v_img_id //in
                          )
{
    uint16 width = 0;
    uint16 height = 0;
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_RECT_T rect = {0};
    
    GUIRES_GetImgWidthHeight(&width, &height, h_img_id, win_id);
    
    rect = both_rect_ptr->h_rect;
    rect.right = rect.left + width - 1;
    rect.bottom = rect.top + height - 1;
    both_rect_ptr->h_rect  = rect;
    
    GUIRES_GetImgWidthHeight(&width, &height, v_img_id, win_id);
    rect = both_rect_ptr->v_rect;
    rect.right = rect.left + width - 1;
    rect.bottom = rect.top + height - 1;
    both_rect_ptr->v_rect  = rect;    
}

/*****************************************************************************/
//Description :  get button capture both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnCaptureBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                 
                                 )
{
    GUI_RECT_T rect = {0};
    
    rect.left = BTN_CAPTURE_H_X;
    rect.top = BTN_CAPTURE_H_Y;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_CAPTURE_V_X;
    rect.top = BTN_CAPTURE_V_Y;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_PHOTO, IMG_BTN_PHOTO_V);
}

/*****************************************************************************/
//Description :  get button sensor both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnSensorBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                
                                )
{
    GUI_RECT_T rect = {0};
    
    rect.left = BTN_SENSOR_H_X;
    rect.top = BTN_SENSOR_H_Y;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_SENSOR_V_X;
    rect.top = BTN_SENSOR_V_Y;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_SENSOR_FRONT, IMG_BTN_SENSOR_FRONT);
}

/*****************************************************************************/
//Description :  get button sensor both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnZoomBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    GUI_RECT_T rect = {0};
    
    rect.left = BTN_ZOOM_H_X;
    rect.top = BTN_ZOOM_H_Y;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_ZOOM_V_X;
    rect.top = BTN_ZOOM_V_Y;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_ZOOM, IMG_BTN_ZOOM);
}

/*****************************************************************************/
//Description :  get button pic both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnPicBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                             )
{
    GUI_RECT_T rect = {0};
    MMI_IMAGE_ID_T img_id = IMG_BTN_PREVIEW_IMG;
#ifdef MMIDC_F_N_IN_ONE
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();

    if(MMIDC_IsNInOneEnable() 
            && (DC_PREVIEW_MODE == mode)
            )
    {
        rect.left = BTN_PIC_NINONE_H_X;
        rect.top = BTN_PIC_NINONE_H_Y;
        both_rect_ptr->h_rect = rect;
        
        rect.left = BTN_PIC_NINONE_V_X;
        rect.top = BTN_PIC_NINONE_V_Y;
        both_rect_ptr->v_rect = rect;
    }
    else
#endif
    {
        rect.left = BTN_PIC_H_X;
        rect.top = BTN_PIC_H_Y;
        both_rect_ptr->h_rect = rect;
        
        rect.left = BTN_PIC_V_X;
        rect.top = BTN_PIC_V_Y;
        both_rect_ptr->v_rect = rect;
    }   
    GetBtnBothRect(both_rect_ptr, img_id, img_id);
}

/*****************************************************************************/
//Description :  get button expand both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnExpandBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                )
{
    GUI_RECT_T rect = {0};
    
    rect.left = BTN_HIDE_EXPAND_H_LEFT;
    rect.top = BTN_HIDE_EXPAND_H_TOP;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_HIDE_EXPAND_V_LEFT;
  //  rect.top = BTNLIST_BG_HIDE_MODE_V_BOTTEOM;//BTN_HIDE_EXPAND_V_TOP;
	 rect.top = BTN_HIDE_EXPAND_V_TOP;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_EXPAND_IN, IMG_ICONLIST_EXPAND_IN_V);
}

/*****************************************************************************/
//Description :  set all cmd buttons visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetAllCmdBtnsVisible(BOOLEAN is_visible)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMI_CTRL_ID_T ctrl_left = MMIDC_PDA_BUTTON_CMD_1_CTRL_ID;
    MMI_CTRL_ID_T ctrl_mid = MMIDC_PDA_BUTTON_CMD_2_CTRL_ID;
    MMI_CTRL_ID_T ctrl_right = MMIDC_PDA_BUTTON_CMD_3_CTRL_ID;
    
    if (PNULL != MMK_GetCtrlHandleByWin(win_id, ctrl_left))
    {
        SetCtrlVisible(ctrl_left, is_visible);
        SetCtrlVisible(ctrl_mid, is_visible);
        SetCtrlVisible(ctrl_right, is_visible);
    }

    //SCI_TRACE_LOW:"[MMIDC] SetAllCmdBtnsVisible is_visible=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3511_112_2_18_2_8_59_278,(uint8*)"d",is_visible);
}

/*****************************************************************************/
//Description :  display cmd button bg bar
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayCmdBtnBgBar(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_IMAGE_ID_T bg_bar_id = IMG_BTN_BAR_BG;
#if !defined(MMIDC_F_PDA_BIT_16)    
    GUI_LCD_DEV_INFO    dev_info = MMIDC_GetOSDBlockInfo();
#endif    
    
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    uint16 img_width = 0;
    uint16 img_height = 0;
    
    GUI_RECT_T rect = {0};  
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        bg_bar_id = IMG_BTN_BAR_BG_V;
        GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_bar_id, win_id);  

        rect.right = screen_width - 1;
        rect.left = 0;

        rect.bottom = screen_height - 1;    
        rect.top = rect.bottom - img_height + 1;
        
    }
    else
    {
        bg_bar_id = IMG_BTN_BAR_BG;
        GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_bar_id, win_id);  

        rect.right = screen_width - 1;
        rect.left = rect.right - img_width + 1;
        
        rect.top = 0;
        rect.bottom = screen_height - 1;    
    }

#if defined(MMIDC_F_PDA_BIT_16)
    MMIDC_ClearRect(rect, MMIDC_PDA_LIST_BG_COLOR);
#else
    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        bg_bar_id,
        &dev_info);
#endif        
}

/*****************************************************************************/
//Description :  set cmd button icon
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetCmdBtnIcon(MMI_CTRL_ID_T ctrl_id,
                         MMI_IMAGE_ID_T fg_img_id
                         )
{
#if !defined(MMIDC_F_PDA_BIT_16)
    MMI_IMAGE_ID_T press_id = IMG_BTN_BG_FOCUS;   
    MMI_IMAGE_ID_T bg_id = IMG_BTN_BG;
#endif    
    
    GUI_BG_T bg_info = {0};
    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = fg_img_id;
    GUIBUTTON_SetFg(ctrl_id,&bg_info);  

#if !defined(MMIDC_F_PDA_BIT_16)    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = bg_id;
    GUIBUTTON_SetBg(ctrl_id,&bg_info); 
#endif    
    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = fg_img_id;
    GUIBUTTON_SetPressedFg(ctrl_id,&bg_info); 

#if !defined(MMIDC_F_PDA_BIT_16)    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = press_id;
    GUIBUTTON_SetPressedBg(ctrl_id,&bg_info);  
#endif    
}

/*****************************************************************************/
//Description :  create cmd buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateAllCmdBtns(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_left = MMIDC_PDA_BUTTON_CMD_1_CTRL_ID;
    MMI_CTRL_ID_T ctrl_mid = MMIDC_PDA_BUTTON_CMD_2_CTRL_ID;
    MMI_CTRL_ID_T ctrl_right = MMIDC_PDA_BUTTON_CMD_3_CTRL_ID;
    MMI_IMAGE_ID_T img_id = IMG_BTN_SAVE;
    
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    uint16 img_width = 0;
    uint16 img_height = 0;
    
    GUI_BOTH_RECT_T   both_rect_left = {0};
    GUI_BOTH_RECT_T   both_rect_mid = {0};
    GUI_BOTH_RECT_T   both_rect_right = {0};
    GUI_RECT_T rect = {0};
    
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);  
    
    
    //left
    rect.right = screen_width - 1;
    rect.left = rect.right - img_width + 1;
    
    rect.bottom = screen_height - 1;    
    rect.top = rect.bottom - img_height + 1;
    
    both_rect_left.h_rect = rect;
    both_rect_left.v_rect = rect;   
    
    CreateCameraBtn(ctrl_left, &both_rect_left, img_id);
    
    //mid
    rect.right = screen_width - 1;
    rect.left = rect.right - img_width + 1;
    
    rect.top = (screen_height - img_height) / 2  - 1;    
    rect.bottom = rect.top + img_height - 1;
    
    both_rect_mid.h_rect = rect;
    both_rect_mid.v_rect = rect;   
    
    CreateCameraBtn(ctrl_mid, &both_rect_mid, img_id);
    
    //right
    rect.right = screen_width - 1;
    rect.left = rect.right - img_width + 1;
    
    rect.top = 0;
    rect.bottom = rect.top + img_height - 1;    
    
    both_rect_right.h_rect = rect;
    both_rect_right.v_rect = rect;   
    
    CreateCameraBtn(ctrl_right, &both_rect_right, img_id);
}

/*****************************************************************************/
//Description :  draw cmd buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DrawCmdBtns(MMI_IMAGE_ID_T img_left,
                       MMI_IMAGE_ID_T img_mid,
                       MMI_IMAGE_ID_T img_right
                       )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_left = MMIDC_PDA_BUTTON_CMD_1_CTRL_ID;
    MMI_CTRL_ID_T ctrl_mid = MMIDC_PDA_BUTTON_CMD_2_CTRL_ID;
    MMI_CTRL_ID_T ctrl_right = MMIDC_PDA_BUTTON_CMD_3_CTRL_ID;
    
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    uint16 img_width = 0;
    uint16 img_height = 0;
    
    GUI_RECT_T rect = {0};
#if defined(MMIDC_F_PDA_BIT_16)
    MMI_IMAGE_ID_T bg_id = IMG_BTN_CANCEL;
#else
    MMI_IMAGE_ID_T bg_id = IMG_BTN_BG;
#endif
    MMI_IMAGE_ID_T bg_bar_id = IMG_BTN_BAR_BG;
    uint16 bar_width = 0;
    uint16 bar_height = 0;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    uint16 x_offset = 0;
    uint16 y_offset = 0;

    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    
    
    DisplayCmdBtnBgBar();
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);    
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, bg_id, win_id);  

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        bg_bar_id = IMG_BTN_BAR_BG_V;
        GUIRES_GetImgWidthHeight(&bar_width, &bar_height, bg_bar_id, win_id);  

        x_offset = BTN_CMD_INTERVAL_V_X;
        y_offset = (bar_height - bg_height) / 2;
    }
    else
    {
        bg_bar_id = IMG_BTN_BAR_BG;
        GUIRES_GetImgWidthHeight(&bar_width, &bar_height, bg_bar_id, win_id);  

        x_offset = (bar_width - bg_width) / 2;
        y_offset = BTN_CMD_INTERVAL_H_Y;
    }
    
    //left
    if (0 != img_left)
    {
        SetCtrlVisible(ctrl_left, TRUE);
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, img_left, win_id);  
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            rect.left = x_offset;
            rect.right = rect.left + img_width - 1;

            rect.bottom = screen_height - 1 - y_offset;
            rect.top = rect.bottom - img_height + 1;
        }
        else
        {
            rect.right = screen_width - 1 - x_offset;
            rect.left = rect.right - img_width + 1;
            
            rect.bottom = screen_height - 1 - y_offset;    
            rect.top = rect.bottom - img_height + 1;
        }
        
        GUIBUTTON_SetRect(ctrl_left, &rect);
        SetCmdBtnIcon(ctrl_left, img_left);        
    }
    else
    {
        SetCtrlVisible(ctrl_left, FALSE);
        SCI_MEMSET(&rect, 0, sizeof(rect));
    }
    MMIDC_SetLeftSoftkey(rect);
    
    //mid
    if (0 != img_mid)
    {
        SetCtrlVisible(ctrl_mid, TRUE);
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, img_mid, win_id);
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            rect.left = (screen_width - img_width) / 2  - 1;;
            rect.right = rect.left + img_width - 1;

            rect.bottom = screen_height - 1 - y_offset;
            rect.top = rect.bottom - img_height + 1;
        }
        else
        {
            rect.right = screen_width - 1 - x_offset;
            rect.left = rect.right - img_width + 1;
            
            rect.top = (screen_height - img_height) / 2  - 1;    
            rect.bottom = rect.top + img_height - 1;
        }
        GUIBUTTON_SetRect(ctrl_mid, &rect);
        SetCmdBtnIcon(ctrl_mid, img_mid);
    }
    else
    {
        SetCtrlVisible(ctrl_mid, FALSE);
        SCI_MEMSET(&rect, 0, sizeof(rect));
    }
    MMIDC_SetMiddleSoftkey(rect);
    
    //right
    if (0 != img_right)
    {
        SetCtrlVisible(ctrl_right, TRUE);
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, img_right, win_id);
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            rect.right = screen_width - x_offset - 1;
            rect.left = rect.right - img_width + 1;;

            rect.bottom = screen_height - 1 - y_offset;
            rect.top = rect.bottom - img_height + 1;
        }
        else
        {
            rect.right = screen_width - 1 - x_offset;
            rect.left = rect.right - img_width + 1;
            
            rect.top = y_offset;
            rect.bottom = rect.top + img_height - 1;    
        }
        GUIBUTTON_SetRect(ctrl_right, &rect);   
        SetCmdBtnIcon(ctrl_right, img_right);
    }
    else
    {
        SetCtrlVisible(ctrl_right, FALSE);
        SCI_MEMSET(&rect, 0, sizeof(rect));
    }
    
    //only set right button to max width
    //for display at other ui
    switch(layout_style)
    {
    case MMIDC_LAYOUT_TRADITION:
        rect.left = 0;
        rect.right = rect.left + img_width - 1;
        
        rect.top = y_offset;
        rect.bottom = rect.top + bar_height - 1;
        break;
    default:
        rect.right = screen_width - 1;
        rect.left = rect.right - bar_width + 1;
        
        rect.top = 0;
        rect.bottom = rect.top + img_height - 1;
        break;
    }
    MMIDC_SetRightSoftkey(rect);
    
    
    DisplayCmdBtns();
}

/*****************************************************************************/
//Description : display arrow buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayArrowBtns(void)
{
    MMI_CTRL_ID_T left_ctrl_id = MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID;
    MMI_CTRL_ID_T right_ctrl_id = MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID;
    
    DC_FLOW_MODE_E flow_mode = 0;
    uint32 total_photo_number = 0;
    uint32 cur_photo_id = 0;
    BOOLEAN is_visible = FALSE;
    BOOLEAN is_left_gray = FALSE;
    BOOLEAN is_right_gray = FALSE;

    flow_mode = MMIDC_GetCurrentMode();
    
    if (DC_SAVE_MODE != flow_mode
        && DC_REVIEW_MODE != flow_mode
       )
    {
        return;
    }

    cur_photo_id = MMIDC_GetCurrentPhotoID();
    total_photo_number = MMIDC_GetCapturedPhotosNumber();
    
    if (1 < total_photo_number)    
    {
        is_visible = TRUE;
        SetArrowLeftBtnVisible(is_visible);
        SetArrowRightBtnVisible(is_visible);
        
        if (0 == cur_photo_id)
        {
            is_left_gray = TRUE;
        }
        if (cur_photo_id == (total_photo_number - 1) )
        {
            is_right_gray = TRUE;
        }
        
        ClearArrowBtnRect();
        ResetArrowLeftBtn(is_left_gray);
        ResetArrowRightBtn(is_right_gray);
        
        MMK_SendMsg(left_ctrl_id, MSG_CTL_PAINT, PNULL);
        MMK_SendMsg(right_ctrl_id, MSG_CTL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//Description : display all cmd buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayAllCmdBtns(DC_FLOW_MODE_E mode)
{
    BOOLEAN is_visible = FALSE;
    MMI_IMAGE_ID_T img_left = 0;
    MMI_IMAGE_ID_T img_mid = 0;
    MMI_IMAGE_ID_T img_right = 0;
    //SCI_TRACE_LOW:"[MMIDC]: CreateAllCmdBtns mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_3914_112_2_18_2_9_0_279,(uint8*)"d", mode);

    SetArrowLeftBtnVisible(is_visible);
    SetArrowRightBtnVisible(is_visible);
    
#if 0 //defined(MMIDC_F_EFFECT_360)
    SetEffect360LeftBtnVisible(is_visible);
    SetEffect360RightBtnVisible(is_visible);
#endif

    switch(mode)
    {
    case DC_PREVIEW_MODE:
        break;
        
    case DC_CAPTURE_CONT_DOWN_MODE:
        img_right = IMG_BTN_CANCEL;
        break;        
    case DC_CAPTURE_MODE:
    case DC_REVIEW_MODE:
#if defined(MMIDC_F_WORK_MODE)
        if (MMIDC_IsWorkMode())
        {
            img_left = IMG_BTN_OK;
            img_mid = IMG_BTN_CANCEL;
            img_right = IMG_BTN_EXIT;
        }
        else
#endif          
        {
            img_left = IMG_BTN_SAVE;
            img_right = IMG_BTN_CANCEL;
#if 0 //defined(MMIDC_F_EFFECT_360)
            img_mid = IMG_BTN_PAUSE;
#endif
        }

        break;
        
    case DC_SAVE_MODE:    
        {
#if defined(MMI_ISTYLE_SUPPORT)			
            if(MMITHEME_IsIstyle())
            {
                img_left = IMG_BTN_OPTION;
                img_right = IMG_BTN_CANCEL;
            }
            else
#endif				
            {
                img_left = IMG_BTN_SEND;
                img_right = IMG_BTN_DELETE;
            }
        }
        DisplayArrowBtns();
        break;
        
    case DV_PREVIEW_MODE:    
        break;
        
    case DV_RECORD_MODE:
        img_left = IMG_BTN_STOP;
        img_mid = IMG_BTN_PAUSE;
        img_right = IMG_BTN_CANCEL;

#if defined(MMIDC_F_DV_ZOOM)
        {
            uint32 max_zoom = MMIDC_GetDVMaxZoomLevel();

            if (max_zoom > 1)
            {
                InitZoomSeekBarInfo(FALSE);
#if defined(MMIDC_F_DV_ZOOM)
                StopZoomSeekBarDisplayTimer();
                StartZoomSeekBarDisplayTimer();

                SetDisplaySeekBarFlag(TRUE);
#endif                
                DisplayZoomSeekBar();   				             
            }
        }
#endif        
        break;
        
    case DV_PAUSE_MODE:
        img_left = IMG_BTN_STOP;
        img_mid = IMG_BTN_RESUME;
        img_right = IMG_BTN_CANCEL;
        break;
        
    case DV_REVIEW_MODE: 
#if defined(MMIDC_F_WORK_MODE)    
        if (MMIDC_IsWorkMode())
        {
            img_left = IMG_BTN_OK;
            img_mid = IMG_BTN_CANCEL;
            img_right = IMG_BTN_EXIT;
        }
        else
#endif  
        {
            img_left = IMG_BTN_SEND;
            img_mid = IMG_BTN_PLAY;
#if defined(MMI_ISTYLE_SUPPORT)			
            if(MMITHEME_IsIstyle())
            {
                img_right = IMG_BTN_CANCEL;
            }
            else
#endif				
            {
                img_right = IMG_BTN_DELETE;
            }
        }
        break;
    default:
        break;
    }
    DrawCmdBtns(img_left, img_mid, img_right);


#if 0 //defined(MMIDC_F_EFFECT_360)
    if (DC_REVIEW_MODE == mode)
    {
        DisplayArrowBtns();
        DisplayEffect360ArrowBtns();
    }
#endif
}
/*****************************************************************************/
//Description :  reset mode button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetModeBtn(void)
{
#if !defined(MMIDC_F_PDA_BIT_16)
    GUI_BG_T bg_info = {0};   
#endif    
    GUI_BG_T fg_info = {0}; 
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID;
    BUTTON_LIST_FOCUS_E focus = GetButtonListFocus();
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    BOOLEAN is_gray = FALSE;

    fg_info.bg_type = GUI_BG_IMG;    
    
    if (focus == BUTTON_LIST_FOCUS_MODE)
    {        
        if(CAMERA_MODE_DC == mode)
        {
            fg_info.img_id = IMG_ICONLIST_PHOTO_FOCUS;
        }
        else
        {
            fg_info.img_id = IMG_ICONLIST_VIDEO_FOCUS;
        }
#if !defined(MMIDC_F_PDA_BIT_16)
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMG_ICONLIST_ICON_FOCUS_BG;
#endif
    }
    else
    {
        if(CAMERA_MODE_DC == mode)
        {
            fg_info.img_id = IMG_ICONLIST_PHOTO;
        }
        else
        {
            fg_info.img_id = IMG_ICONLIST_VIDEO;
        }
    }
    
#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        is_gray = TRUE;
        if(CAMERA_MODE_DC == mode)
        {
            fg_info.img_id = IMG_ICONLIST_PHOTO_GRAY;
        }
        else
        {
            fg_info.img_id = IMG_ICONLIST_VIDEO_GRAY;
        }
    }
#endif

    GUIBUTTON_SetGrayed(ctrl_id,is_gray,&fg_info,FALSE);

    GUIBUTTON_SetFg(ctrl_id,&fg_info);  
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIBUTTON_SetBg(ctrl_id,&bg_info);   
#endif        
}
/*****************************************************************************/
//Description :  get button mode both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnModeBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    GUI_RECT_T rect = {0};
    BOOLEAN is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    
    
    if(is_press)
    {
        rect.left = BTN_SHOW_MODE_H_LEFT;
        rect.top = BTN_SHOW_MODE_H_TOP;
    }
    else
    {
        rect.left = BTN_HIDE_MODE_H_LEFT;
        rect.top = BTN_HIDE_MODE_H_TOP;
    }
    both_rect_ptr->h_rect = rect;
    
    if(is_press)
    {
        rect.left = BTN_SHOW_MODE_V_LEFT;
        rect.top = BTN_SHOW_MODE_V_TOP;
    }
    else
    {
        rect.left = BTN_HIDE_MODE_V_LEFT;
        rect.top = BTN_HIDE_MODE_V_TOP;
    }   
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_PHOTO, IMG_ICONLIST_PHOTO);
}

/*****************************************************************************/
//Description :  create mode button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateModeBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnModeBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID, &both_rect, IMG_ICONLIST_PHOTO);
    ResetModeBtn();
}

/*****************************************************************************/
//Description :  destroy mode btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DestroyModeBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID;
    
    MMK_DestroyControl(ctrl_id);
}

/*****************************************************************************/
//Description :  set mode btn visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetModeBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}
/*****************************************************************************/
//Description :  reset ImageProperty button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetImagePropertyBtn(void)
{
#if !defined(MMIDC_F_PDA_BIT_16)
    GUI_BG_T bg_info = {0}; 
#endif    
    GUI_BG_T fg_info = {0}; 
    BUTTON_LIST_FOCUS_E focus = GetButtonListFocus();
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID;
    
    fg_info.bg_type = GUI_BG_IMG;
    if (focus == BUTTON_LIST_FOCUS_IMAGE_PROPERTY)
    {
        fg_info.img_id = IMG_ICONLIST_PHOTO_ATTRIBUTE_FOCUS;    
#if !defined(MMIDC_F_PDA_BIT_16)                  
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMG_ICONLIST_ICON_FOCUS_BG;
#endif
    }
    else
    {
        fg_info.img_id = IMG_ICONLIST_PHOTO_ATTRIBUTE;
    }

    GUIBUTTON_SetFg(ctrl_id,&fg_info);  
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIBUTTON_SetBg(ctrl_id,&bg_info);    
#endif       
    
}
/*****************************************************************************/
//Description :  get button ImageProperty both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnImagePropertyBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;

    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T rect = {0};
    MMI_CTRL_ID_T pre_ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID;
    MMI_IMAGE_ID_T pre_img_id = IMG_ICONLIST_PHOTO;
    uint16 width = 0;
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(&width, &height, pre_img_id, win_id);
    GUIAPICTRL_GetBothRect(pre_ctrl_id ,&both_rect);

    rect = both_rect.h_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->h_rect = rect; 

    rect = both_rect.v_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_PHOTO_ATTRIBUTE, IMG_ICONLIST_PHOTO_ATTRIBUTE);
}

/*****************************************************************************/
//Description :  create ImageProperty button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateImagePropertyBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnImagePropertyBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID, &both_rect, IMG_ICONLIST_PHOTO_ATTRIBUTE);
    ResetImagePropertyBtn();
}

/*****************************************************************************/
//Description :  destroy ImageProperty btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DestroyImagePropertyBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID;
    
    MMK_DestroyControl(ctrl_id);
}

/*****************************************************************************/
//Description :  set ImageProperty btn visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetImagePropertyBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}
/*****************************************************************************/
//Description :  reset WhiteBlance button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetWhiteBlanceBtn(void)
{
#if !defined(MMIDC_F_PDA_BIT_16)
    GUI_BG_T bg_info = {0};    
#endif    
    GUI_BG_T fg_info = {0}; 
    BUTTON_LIST_FOCUS_E focus = GetButtonListFocus();
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID;
    BOOLEAN is_gray = TRUE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();

    fg_info.bg_type = GUI_BG_IMG;
    if (focus == BUTTON_LIST_FOCUS_WHITEBLANCE)
    {
        fg_info.img_id = IMG_ICONLIST_WB_FOCUS;   
#if !defined(MMIDC_F_PDA_BIT_16)             
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMG_ICONLIST_ICON_FOCUS_BG;
#endif
    }
    else
    {
        fg_info.img_id = IMG_ICONLIST_WB;
    }
        
    if (CAMERA_MODE_DC == mode)
    {
        is_gray = FALSE;
        GUIBUTTON_SetGrayed(ctrl_id,is_gray,&fg_info,FALSE);
    }
    else
    {
        is_gray = TRUE;
        fg_info.img_id = IMG_ICONLIST_WB_GRAY;
        GUIBUTTON_SetGrayed(ctrl_id,is_gray,&fg_info,FALSE);
    }
    GUIBUTTON_SetFg(ctrl_id,&fg_info);  
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIBUTTON_SetBg(ctrl_id,&bg_info);    
#endif       
    
}
/*****************************************************************************/
//Description :  get button WhiteBlance both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnWhiteBlanceBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;

    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T rect = {0};
    MMI_CTRL_ID_T pre_ctrl_id = MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID;
    MMI_IMAGE_ID_T pre_img_id = IMG_ICONLIST_PHOTO;
    uint16 width = 0;
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(&width, &height, pre_img_id, win_id);
    GUIAPICTRL_GetBothRect(pre_ctrl_id ,&both_rect);

    rect = both_rect.h_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->h_rect = rect; 

    rect = both_rect.v_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_WB, IMG_ICONLIST_WB);
}

/*****************************************************************************/
//Description :  create WhiteBlance button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateWhiteBlanceBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnWhiteBlanceBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID, &both_rect, IMG_ICONLIST_WB);
    ResetWhiteBlanceBtn();
}

/*****************************************************************************/
//Description :  destroy WhiteBlance btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DestroyWhiteBlanceBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID;
    
    MMK_DestroyControl(ctrl_id);
}

/*****************************************************************************/
//Description :  set WhiteBlance btn visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetWhiteBlanceBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}
/*****************************************************************************/
//Description :  reset Effect button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetEffectBtn(void)
{
#if !defined(MMIDC_F_PDA_BIT_16)
    GUI_BG_T bg_info = {0}; 
#endif    
    GUI_BG_T fg_info = {0}; 
    BUTTON_LIST_FOCUS_E focus = GetButtonListFocus();
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID;
    
    fg_info.bg_type = GUI_BG_IMG;
    if (focus == BUTTON_LIST_FOCUS_EFFECT)
    {
        fg_info.img_id = IMG_ICONLIST_EFFECT_FOCUS;
#if !defined(MMIDC_F_PDA_BIT_16)
        bg_info.img_id = IMG_ICONLIST_ICON_FOCUS_BG;
        bg_info.bg_type = GUI_BG_IMG;
#endif
    }
    else
    {
        fg_info.img_id = IMG_ICONLIST_EFFECT;
    }

    GUIBUTTON_SetFg(ctrl_id,&fg_info); 
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIBUTTON_SetBg(ctrl_id,&bg_info);   
#endif        
}
/*****************************************************************************/
//Description :  get button Effect both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnEffectBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;

    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T rect = {0};
    MMI_CTRL_ID_T pre_ctrl_id = MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID;
    MMI_IMAGE_ID_T pre_img_id = IMG_ICONLIST_PHOTO;
    uint16 width = 0;
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(&width, &height, pre_img_id, win_id);
    GUIAPICTRL_GetBothRect(pre_ctrl_id ,&both_rect);

    rect = both_rect.h_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->h_rect = rect; 

    rect = both_rect.v_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_EFFECT, IMG_ICONLIST_EFFECT);
}

/*****************************************************************************/
//Description :  create Effect button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateEffectBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnEffectBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID, &both_rect, IMG_ICONLIST_EFFECT);
    ResetEffectBtn();
}

/*****************************************************************************/
//Description :  destroy Effect btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DestroyEffectBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID;
    
    MMK_DestroyControl(ctrl_id);
}

/*****************************************************************************/
//Description :  set Effect btn visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetEffectBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}
/*****************************************************************************/
//Description :  reset Setting button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetSettingBtn(void)
{
#if !defined(MMIDC_F_PDA_BIT_16)
    GUI_BG_T bg_info = {0}; 
#endif    
    GUI_BG_T fg_info = {0}; 
    BUTTON_LIST_FOCUS_E focus = GetButtonListFocus();
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID;
#if defined(MMIDC_F_U_CAMERA)   
    GUI_BOTH_RECT_T   both_rect = {0};    
#endif

    fg_info.bg_type = GUI_BG_IMG;
    if (focus == BUTTON_LIST_FOCUS_SETTING)
    {
        fg_info.img_id = IMG_ICONLIST_SETTINGS_FOCUS;
#if !defined(MMIDC_F_PDA_BIT_16)
        bg_info.img_id = IMG_ICONLIST_ICON_FOCUS_BG;
        bg_info.bg_type = GUI_BG_IMG;
#endif
    }
    else
    {
        fg_info.img_id = IMG_ICONLIST_SETTINGS;
    }

    GUIBUTTON_SetFg(ctrl_id,&fg_info); 
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIBUTTON_SetBg(ctrl_id,&bg_info);
#endif 

/*Jessica add - begin*/
#if defined(MMIDC_F_U_CAMERA)
    if(MMIDC_IsUCameraEnable())
    {
        GetBtnModeBothRect(&both_rect);
    }
    else
    {
        GetBtnSettingBothRect(&both_rect);
    }
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)    
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.h_rect);
    }
    else
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.v_rect);
    }

#endif

/*Jessica add - end*/

}
/*****************************************************************************/
//Description :  get button Setting both rect
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void GetBtnSettingBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                              )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;

    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T rect = {0};
    MMI_CTRL_ID_T pre_ctrl_id = MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID;
    MMI_IMAGE_ID_T pre_img_id = IMG_ICONLIST_PHOTO;
    uint16 width = 0;
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(&width, &height, pre_img_id, win_id);
    GUIAPICTRL_GetBothRect(pre_ctrl_id ,&both_rect);

    rect = both_rect.h_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->h_rect = rect; 

    rect = both_rect.v_rect;
    rect.top = rect.top + height;
    rect.bottom = rect.bottom + height;    
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_ICONLIST_SETTINGS, IMG_ICONLIST_SETTINGS);
}

/*****************************************************************************/
//Description :  create Setting button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateSettingBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnSettingBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID, &both_rect, IMG_ICONLIST_SETTINGS);
    ResetSettingBtn();
}

/*****************************************************************************/
//Description :  destroy Setting btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DestroySettingBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID;
    
    MMK_DestroyControl(ctrl_id);
}

/*****************************************************************************/
//Description :  set Setting btn visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetSettingBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}
/*****************************************************************************/
//Description : buttonlist bg
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawButtonListBgBmp(void)
{    
    BOOLEAN is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E  index = PDA_RECT_BTN_LIST_BG;
    
    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);

    if (is_press)
    {
        #if defined(MMIDC_F_PDA_BIT_16)
        {
            GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
        
            LCD_FillRect(&dev_info, rect, MMIDC_PDA_LIST_BG_COLOR);
        }
        #else
        {
            MMI_IMAGE_ID_T img_id = IMG_ICONLIST_BG;
  
            MMIDC_DisplayBmp(rect.left, rect.top,img_id);
        }
        #endif
    }

}

/*****************************************************************************/
//Description :  create  buttonlist
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void CreateButtonList(void)
{
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E item = 0;

    item = PDA_RECT_BTN_LIST_BG;
    rect = GetItemFixedRect(item, FALSE);
    SetIconRect(item, rect);
    
    CreateModeBtn();
    CreateImagePropertyBtn();
    CreateWhiteBlanceBtn();    
    CreateEffectBtn();
    CreateSettingBtn();
}

/*****************************************************************************/
//Description :  set buttonlist visible
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void SetButtonListVisible(BOOLEAN is_visible)
{
    SetModeBtnVisible(is_visible);
    SetImagePropertyBtnVisible(is_visible);
    SetWhiteBlanceBtnVisible(is_visible);
    SetEffectBtnVisible(is_visible);
    SetSettingBtnVisible(is_visible);
}
/*****************************************************************************/
//Description :  reset buttonlist 
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ResetButtonList()
{
    ResetModeBtn();
    ResetImagePropertyBtn();
    ResetWhiteBlanceBtn();
    ResetEffectBtn();
    ResetSettingBtn();
}

/*****************************************************************************/
//Description :  reset capture button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetCaptureBtn(CAMERA_MODE_E mode)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    MMI_IMAGE_ID_T bg_id = 0;
    MMI_IMAGE_ID_T press_id = 0;
    MMI_IMAGE_ID_T gray_id = 0;

    BOOLEAN is_expand_press = FALSE;
    BOOLEAN is_gray = FALSE;

    is_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (is_expand_press)
    {
        is_gray = TRUE;
    }
    
    if (CAMERA_MODE_DC == mode)
    {
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL) 
        {
            bg_id = IMG_BTN_PHOTO;
            press_id = IMG_BTN_PHOTO_FOCUS;
            gray_id = IMG_BTN_PHOTO_GRAY;
        }
        else
        {
            bg_id = IMG_BTN_PHOTO_V;
            press_id = IMG_BTN_PHOTO_FOCUS_V;
            gray_id = IMG_BTN_PHOTO_GRAY_V;
        }
    }
    else
    {
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL) 
        {
            bg_id = IMG_BTN_VIDEO;
            press_id = IMG_BTN_VIDEO_FOCUS;
            gray_id = IMG_BTN_VIDEO_GRAY;
        }
        else
        {
            bg_id = IMG_BTN_VIDEO_V;
            press_id = IMG_BTN_VIDEO_FOCUS_V;
            gray_id = IMG_BTN_VIDEO_GRAY_V;
        }
    }
    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = press_id;
    GUIBUTTON_SetPressedBg(ctrl_id,&bg_info);  
    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = bg_id;
    GUIBUTTON_SetBg(ctrl_id,&bg_info);

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = gray_id;
    GUIBUTTON_SetGrayed(ctrl_id,is_gray,&bg_info,FALSE);

}

/*****************************************************************************/
//Description :  create capture button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateCaptureBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    GetBtnCaptureBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID, &both_rect, IMG_BTN_PHOTO);
    
    ResetCaptureBtn(mode);
}

/*****************************************************************************/
//Description :  set capture btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetCaptureBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset sensor button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetSensorBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_SENSOR_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    BOOLEAN is_expand_press = FALSE;
    BOOLEAN is_gray = FALSE;

#if defined(MMIDC_F_U_CAMERA)
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    GUI_BOTH_RECT_T both_rect = {0};
#endif

    is_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (is_expand_press)
    {
        is_gray = TRUE;
    }

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_SENSOR_FRONT_FOCUS;
    GUIBUTTON_SetPressedBg(ctrl_id,&bg_info); 

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMG_BTN_SENSOR_GRAY;
    GUIBUTTON_SetGrayed(ctrl_id,is_gray,&bg_info,FALSE); 

#if defined(MMIDC_F_U_CAMERA)
    if(MMIDC_IsUCameraEnable() 
            && (DC_PREVIEW_MODE == mode)
            )
    {
        GetBtnZoomBothRect(&both_rect);
    }
    else
    {
        GetBtnSensorBothRect(&both_rect);
    }
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)    
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.h_rect);
    }
    else
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.v_rect);
    }
#endif    
}

/*****************************************************************************/
//Description :  create sensor button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateSensorBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnSensorBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_SENSOR_CTRL_ID, &both_rect, IMG_BTN_SENSOR_FRONT);
    
    ResetSensorBtn();
}

/*****************************************************************************/
//Description :  set sensor btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetSensorBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_SENSOR_CTRL_ID;
    BOOLEAN is_show = FALSE;
    int32  sensor_number = 0;

    sensor_number = MMIDC_GetDCameraSensorNumber();
    
    if (MAX_MMI_SENSOR_NUM <= sensor_number)
    {    
        is_show = is_visible;
    }
    else
    {
        is_show = FALSE;
    }
    
    SetCtrlVisible(ctrl_id, is_show);
}

/*****************************************************************************/
//Description :  reset zoom button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetZoomBtn(void)
{
    GUI_BG_T bg_info = {0}; 
    BOOLEAN is_press = FALSE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    uint32 max_zoom = 0;
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ZOOM_CTRL_ID;
    BOOLEAN is_gray = FALSE;
    
    MULTI_SHOOT_E shoot = 0;

    if (CAMERA_MODE_DC == mode)
    {
        shoot = MMIDC_GetMultiShootEnum();
        max_zoom = MMIDC_GetMaxZoomLevel();
        is_press = GetIconStateAsPress(PDA_RECT_ZOOM);
        
        bg_info.bg_type = GUI_BG_IMG;
        if (is_press)
        {
            bg_info.img_id = IMG_BTN_ZOOM_FOCUS;
        }
        else
        {
            bg_info.img_id = IMG_BTN_ZOOM;
        }

        if ((max_zoom <= 1)
            //|| (MULTI_SHOOT_DISABLE != shoot)
            )
        {
            bg_info.img_id = IMG_BTN_ZOOM_GRAY;
            is_gray = TRUE;
        }

        GUIBUTTON_SetGrayed(ctrl_id,is_gray,&bg_info,FALSE);
        GUIBUTTON_SetBg(ctrl_id,&bg_info);   
    }
    else
    {
#if defined(MMIDC_F_DV_ZOOM)
        max_zoom = MMIDC_GetDVMaxZoomLevel();
        is_press = GetIconStateAsPress(PDA_RECT_ZOOM);
        
        bg_info.bg_type = GUI_BG_IMG;
        if (is_press)
        {
            bg_info.img_id = IMG_BTN_ZOOM_FOCUS;
        }
        else
        {
            bg_info.img_id = IMG_BTN_ZOOM;
        }

        if (max_zoom <= 1)
        {
            bg_info.img_id = IMG_BTN_ZOOM_GRAY;
            is_gray = TRUE;
        }

        GUIBUTTON_SetGrayed(ctrl_id,is_gray,&bg_info,FALSE);
        GUIBUTTON_SetBg(ctrl_id,&bg_info);   
#else        
        SetZoomBtnVisible(FALSE);
#endif        
    }

#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag()
        && (CAMERA_MODE_DC == mode)
        )
    {
        SetZoomBtnVisible(FALSE);
    }
#endif
}

/*****************************************************************************/
//Description :  create zoom button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateZoomBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnZoomBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_ZOOM_CTRL_ID, &both_rect, IMG_BTN_ZOOM);
    
    ResetZoomBtn();
}

/*****************************************************************************/
//Description :  set zoom btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetZoomBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ZOOM_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  get button arrow left both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnArrowLeftBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                
                                )
{
    GUI_RECT_T rect = {0};

    uint16 img_height = 0;
#if !defined(MMIDC_F_PDA_BIT_16)
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;

    uint16 img_width = 0;
    MMI_IMAGE_ID_T bg_id = IMG_BTN_BG;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_id, win_id);  
#endif
    
    rect.left = BTN_ARROW_LEFT_H_X;
    rect.top = BTN_ARROW_LEFT_H_Y;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_ARROW_LEFT_V_X;
    rect.top = img_height + 30;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_ARROW_LEFT_FOCUS, IMG_BTN_ARROW_LEFT_FOCUS);
}

/*****************************************************************************/
//Description :  set arrow left btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetArrowLeftBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset arrow left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetArrowLeftBtn(BOOLEAN is_gray
                             )
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    GUI_BG_T fg_info = {0}; 

    fg_info.bg_type = GUI_BG_IMG;    
    fg_info.img_id = IMG_BTN_ARROW_LEFT_FOCUS;
    

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_ARROW_LEFT;

    if(is_gray)
    {
        bg_info.img_id = IMG_BTN_ARROW_LEFT_GRAY;
    }
    GUIBUTTON_SetGrayed(ctrl_id, is_gray, &bg_info, FALSE);
    
    GUIBUTTON_SetPressedFg(ctrl_id,&fg_info);    
    GUIBUTTON_SetBg(ctrl_id,&bg_info); 
}

/*****************************************************************************/
//Description :  create arrow left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateArrowLeftBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnArrowLeftBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID, &both_rect, IMG_BTN_ARROW_LEFT_FOCUS);
    
    ResetArrowLeftBtn(FALSE);
}

/*****************************************************************************/
//Description :  get button arrow right both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnArrowRightBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                
                                )
{
    GUI_RECT_T rect = {0};
    MMI_IMAGE_ID_T img_id = IMG_BTN_ARROW_RIGHT_FOCUS;
    
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID;

    GUIAPICTRL_GetBothRect(ctrl_id, &both_rect);

    rect = both_rect.h_rect;
    rect.left = rect.right + 1 + BTN_ARROW_INTERVAL_H_X;
    both_rect_ptr->h_rect = rect;

    rect = both_rect.v_rect;
    rect.left = rect.right + 1 + BTN_ARROW_INTERVAL_H_X;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, img_id, img_id);
}

/*****************************************************************************/
//Description :  set arrow right btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetArrowRightBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset arrow right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetArrowRightBtn(BOOLEAN is_gray
                              )
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    GUI_BG_T fg_info = {0}; 
    
    fg_info.bg_type = GUI_BG_IMG;    
    fg_info.img_id = IMG_BTN_ARROW_RIGHT_FOCUS;

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_ARROW_RIGHT;

    if(is_gray)
    {
        bg_info.img_id = IMG_BTN_ARROW_RIGHT_GRAY;
    }
    GUIBUTTON_SetGrayed(ctrl_id, is_gray, &bg_info, FALSE);
    
    GUIBUTTON_SetPressedFg(ctrl_id,&fg_info);    
    GUIBUTTON_SetBg(ctrl_id,&bg_info);
}

/*****************************************************************************/
//Description :  create arrow right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateArrowRightBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnArrowRightBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID, &both_rect, IMG_BTN_ARROW_RIGHT_FOCUS);
    
    ResetArrowRightBtn(FALSE);
}

/*****************************************************************************/
//Description :  reset pic button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetPicBtn(void)
{
#ifdef PIC_VIEWER_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_PIC_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    BOOLEAN is_expand_press = FALSE;
    BOOLEAN is_gray = FALSE;
    
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T gray_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T focus_img_id = IMAGE_NULL;

#if defined (MMIDC_F_N_IN_ONE)
    GUI_BOTH_RECT_T both_rect = {0};
#endif

    is_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (is_expand_press)
    {
        is_gray = TRUE;
    }

    if (CAMERA_MODE_DC == mode)
    {
        img_id = IMG_BTN_PREVIEW_IMG;
        focus_img_id = IMG_BTN_PREVIEW_IMG_FOCUS;
        gray_img_id = IMG_BTN_PREVIEW_IMG_GRAY;
    }
    else
    {
        img_id = IMG_BTN_PREVIEW_IMG_VIDEO;
        focus_img_id = IMG_BTN_PREVIEW_IMG_VIDEO_FOCUS;
        gray_img_id = IMG_BTN_PREVIEW_IMG_VIDEO_GRAY;
    }

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = img_id;
    GUIBUTTON_SetBg(ctrl_id, &bg_info);  
    
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = focus_img_id;
    GUIBUTTON_SetPressedBg(ctrl_id, &bg_info);  

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = gray_img_id;
    GUIBUTTON_SetGrayed(ctrl_id,is_gray, &bg_info, FALSE);

#if defined (MMIDC_F_N_IN_ONE)
    GetBtnPicBothRect(&both_rect);
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)    
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.h_rect);
    }
    else
    {
        GUIBUTTON_SetRect(ctrl_id, &both_rect.v_rect);
    }
#endif
#endif
    return;

}

#ifdef PIC_VIEWER_SUPPORT
/*****************************************************************************/
//Description :  create pic button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreatePicBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnPicBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_PIC_CTRL_ID, &both_rect, IMG_BTN_PREVIEW_IMG);
    
    ResetPicBtn();

#ifdef MMIDC_F_LAST_IMAGE
    GUIBUTTON_SetOwnerDraw(MMIDC_PDA_BUTTON_PIC_CTRL_ID, LastThumbnailOwnerDrawFun);
#endif
}
#endif

/*****************************************************************************/
//Description :  set pic btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetPicBtnVisible(BOOLEAN is_visible)
{
#ifdef PIC_VIEWER_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_PIC_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
#endif
    return;
}

/*****************************************************************************/
//Description :  set expand btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetExpandBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EXPAND_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset expand button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetExpandBtn(void)
{
    GUI_BG_T bg_info = {0}; 
    BOOLEAN is_press = FALSE;
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    
    bg_info.bg_type = GUI_BG_IMG;
    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        if(is_press)
        {
            bg_info.img_id  = IMG_ICONLIST_EXPAND_IN_V;
        }
        else
        {
            bg_info.img_id  = IMG_ICONLIST_EXPAND_OUT_V;
        }    
    }
    else
    {
        if(is_press)
        {
            bg_info.img_id  = IMG_ICONLIST_EXPAND_IN;
        }
        else
        {
            bg_info.img_id  = IMG_ICONLIST_EXPAND_OUT;
        }    
    }
    GUIBUTTON_SetBg(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &bg_info);  
}

/*****************************************************************************/
//Description :  reset expand button rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetExpandBtnRect(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EXPAND_CTRL_ID;
    BOOLEAN is_press = FALSE;
    GUI_RECT_T rect = {0};
    
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (!is_press)
    {
        rect = GetItemFixedRect(PDA_RECT_EXPAND, FALSE);
        GUIAPICTRL_SetRect(ctrl_id, &rect);
    }
}

/*****************************************************************************/
//Description :  create expand button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateExpandBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    MMI_IMAGE_ID_T img_id = 0;
    SCREEN_MODE_E screen_mode = 0;

    screen_mode = MMIDC_GetScreenMode();
    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        img_id = IMG_ICONLIST_EXPAND_OUT_V;
    }
    else
    {
        img_id = IMG_ICONLIST_EXPAND_OUT;
    }
    GetBtnExpandBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID, &both_rect, img_id); 
  
    SetIconStateAsPress(PDA_RECT_EXPAND, FALSE);
    ResetExpandBtn();
}

/*****************************************************************************/
//Description :  create pic button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateIconTypeList(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
    MMIDC_PDA_ICON_RECT_E i = 0;
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_E icon_id = 0;

    for (i = PDA_RECT_LIST_SHADOW; i <= PDA_RECT_PIC_PROPERTY_DV_CONTRAST_ICON; i++)
    {
        rect = GetItemFixedRect(i, FALSE);
        SetIconRect(i, rect);
    }

#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        icon_id = PDA_ICON_SETTING;
    }
    else
#endif
#if defined(MMIDC_F_U_CAMERA)
    if(MMIDC_GetUCameraModuleFlag())
    {
        icon_id = PDA_ICON_SETTING;
    }
    else
#endif
    {
        icon_id = PDA_ICON_MODE;
    }

    MMIDC_CreateListCtrl(ctrl_id, icon_id);
    
    ResetIconTypeList(ctrl_id);

    DisplayListCtrl(ctrl_id, icon_id);

}

/*****************************************************************************/
//Description :  destroy icon type list ctrl
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DestroyIconTypeListCtrl(void)
{
    MMK_DestroyControl(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID);
    MMK_DestroyControl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
}

/*****************************************************************************/
//Description : create expand panel
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateExpandPanel(void)
{
    BUTTON_LIST_FOCUS_E focus_button = 0;
    
    CreateExpandBtn();

#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        focus_button = BUTTON_LIST_FOCUS_SETTING;
    }
    else
#endif
#if defined(MMIDC_F_U_CAMERA)
    if(MMIDC_IsUCameraEnable())
    {
        focus_button = BUTTON_LIST_FOCUS_SETTING;
    }
    else
#endif
    {
        focus_button = BUTTON_LIST_FOCUS_MODE;
    }
    SetButtonListFocus(focus_button);
    
    CreateButtonList();
    
    CreateIconTypeList();

    CreateImageAttrInfo();    

    SetExpandPanelVisible(FALSE);
    SetExpandBtnVisible(TRUE);
}

/*****************************************************************************/
//Description : set expand panel visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetExpandPanelVisible(BOOLEAN is_visible)
{
    SetExpandBtnVisible(is_visible);
    SetButtonListVisible(is_visible);
#if defined(MMIDC_F_U_CAMERA)
    if(MMIDC_IsUCameraEnable())
    {
        SetModeBtnVisible(FALSE);
        SetImagePropertyBtnVisible(FALSE);
        SetWhiteBlanceBtnVisible(FALSE);
        SetEffectBtnVisible(FALSE);
    }
#endif
    SetCtrlVisible(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, is_visible);
    SetCtrlVisible(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, is_visible);
    SetCtrlVisible(MMIDC_PDA_LABEL_CTRL_ID, is_visible);
}


#if defined (MMIDC_F_U_CAMERA)
/*****************************************************************************/
//Description :  get button ucamera both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetListItem(void)
{
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    MMIDC_PDA_ICON_E icon_id = PDA_ICON_MODE;
    BUTTON_LIST_FOCUS_E focus = 0;

    DestroyIconTypeListCtrl();

    if(MMIDC_GetUCameraModuleFlag())
    {
         icon_id = PDA_ICON_SETTING;
         focus = BUTTON_LIST_FOCUS_SETTING;
    }
    else
    {
         icon_id = PDA_ICON_MODE;
         focus = BUTTON_LIST_FOCUS_MODE;
    }
    GetListCtrlItem(&list_item);
    list_item.icon_id = icon_id;
    SetListCtrlItem(&list_item);

    SetButtonListFocus(focus);
    MMIDC_CreateListCtrl(MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, list_item.icon_id);    
 }
/*****************************************************************************/
//Description :  get button ucamera both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnUCameraBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                 
                                 )
{
    GUI_RECT_T rect = {0};
    
    rect.left = BTN_UCAMERA_H_X;
    rect.top = BTN_UCAMERA_H_Y;
    both_rect_ptr->h_rect = rect;
    
    rect.left = BTN_UCAMERA_V_X;
    rect.top = BTN_UCAMERA_V_Y;
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_UCAMERA, IMG_BTN_UCAMERA);
}


/*****************************************************************************/
//Description :  reset ucamera button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetUCameraBtn(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    BOOLEAN is_expand_press = FALSE;
    BOOLEAN is_gray = FALSE;   

    is_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (is_expand_press)
    {
        is_gray = TRUE;
    }

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_UCAMERA_FOCUS;
    GUIBUTTON_SetPressedBg(ctrl_id,&bg_info); 

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMG_BTN_UCAMERA_GRAY;
    GUIBUTTON_SetGrayed(ctrl_id,is_gray,&bg_info,FALSE);    
}


/*****************************************************************************/
//Description :  create ucamera button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateUCameraBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};

    GetBtnUCameraBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID, &both_rect, IMG_BTN_UCAMERA);
    
    ResetUCameraBtn();
    
}

/*****************************************************************************/
//Description :  set ucamera btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetUCameraBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

#endif


/*****************************************************************************/
//Description :  create pda desktop all items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void CreatePDAAllItems(void)
{
    CreateExpandPanel();    
    
    CreateCaptureBtn();
    
    CreateSensorBtn();
    
    CreateZoomBtn();

#ifdef PIC_VIEWER_SUPPORT
    CreatePicBtn();
#endif
    CreateAllCmdBtns();

    CreateArrowLeftBtn();

    CreateArrowRightBtn();

#if defined (MMIDC_F_U_CAMERA)
    CreateUCameraBtn();
#endif

#if 0//defined(MMIDC_F_EFFECT_360)
    CreateEffect360LeftBtn();
    CreateEffect360RightBtn();
#endif    
}

/*****************************************************************************/
//Description :  display panel items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayPanelItems(DC_FLOW_MODE_E mode,
                                    BOOLEAN is_show
                                    )
{
    BOOLEAN ret = TRUE;
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    MMIDC_LIST_CTRL_ITEM_T item = {0};
    MMIDC_PDA_ICON_E icon_id = 0;
    uint32 i = 0;

    BOOLEAN is_ctrl_visible = FALSE;
    BOOLEAN is_zoom_visible = FALSE;
    BOOLEAN is_zoom_press = FALSE;


    BOOLEAN is_expand_press = FALSE;
    BUTTON_LIST_FOCUS_E focus = 0;
    BOOLEAN is_expand_visible = FALSE;

    //first, display bg    
    if (PNULL != MMK_GetCtrlHandleByWin(win_id, MMIDC_PDA_BUTTON_EXPAND_CTRL_ID))
    {
        is_expand_visible = IsCtrlVisible(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
        is_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);
        focus = GetButtonListFocus();
        
        //check expand button press
        if(is_expand_press && is_expand_visible)
        {
            DrawPicBoundaryBmp();
            DrawButtonListBgBmp();
            
            if(BUTTON_LIST_FOCUS_IMAGE_PROPERTY == focus)
            {
                DisplayImageAttrAllSeekBar();
            }
        }
    }
    
    for (i = MMIDC_PDA_CTRL_ID_START; i < MMIDC_PDA_CTRL_ID_END; i++)
    {
        if (PNULL != MMK_GetCtrlHandleByWin(win_id, i))
        {
            is_ctrl_visible = IsCtrlVisible(i);
            if (is_ctrl_visible)
            {
                switch(i)
                {
                case MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID:
                case MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID:
                    GetListCtrlItem(&item);
                    icon_id = item.icon_id;
                    DisplayListCtrl(i, icon_id);
                    break;
                    
                default:
                    break;
                }
                
                MMK_SendMsg(i, MSG_CTL_PAINT, PNULL);
            }
        }
    }

#ifdef MMIDC_F_N_IN_ONE
    if(MMIDC_IsNInOneEnable() 
            && (DC_PREVIEW_MODE == mode)
            )
    {
        MMIDC_DisplayNInOne();
    }
#endif


    //check zoom seekbar state to display
    if (PNULL != MMK_GetCtrlHandleByWin(win_id, MMIDC_PDA_BUTTON_ZOOM_CTRL_ID))
    {
        is_zoom_visible = IsCtrlVisible(MMIDC_PDA_BUTTON_ZOOM_CTRL_ID);
        is_zoom_press = GetIconStateAsPress(PDA_RECT_ZOOM);
        if (is_zoom_visible
            && is_zoom_press
           )
        {
            DisplayZoomSeekBar();
        }
    }
    return ret;
}

/*****************************************************************************/
//Description :  display all items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN DisplayPDAAllItems(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    MMIDC_SHOW_STATE_E   show_state = MMIDC_GetShowState();
    int32 count_down_seconds = 0;
    BOOLEAN is_show = FALSE;
    BOOLEAN is_show_img = FALSE;

    
    //SCI_TRACE_LOW:"[MMIDC] DisplayPDAAllItems cur_mode = %d, show_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_5680_112_2_18_2_9_4_280,(uint8*)"dd", mode, show_state);
    
    //display last frame
    if(DV_REVIEW_MODE == mode)
    {
        MMIDC_DisplayLastFrame();
    }
    
    //which flow mode to show content or not
    switch(mode)
    {
    case DC_PREVIEW_MODE:
    case DV_PREVIEW_MODE:
        is_show = TRUE;
        is_show_img = TRUE;
        MMIDC_RestorePDADesktop();
        break;
    case DV_RECORD_MODE:
    case DV_REVIEW_MODE:
    case DV_PAUSE_MODE:
        is_show = TRUE;
        break;
    case DC_SAVE_MODE:
        is_show = TRUE;
        break;
    default:
        break;
    }
    //SCI_TRACE_LOW:"[MMIDC] DisplayPDAAllItems is_show = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_5708_112_2_18_2_9_4_281,(uint8*)"d", is_show);
    
    //if show content ,then display softkey
    if (is_show)
    {
        //MMIDC_DisplaySoftKey(mode);
    }
    
    //display content by flow mode
    switch(mode)
    {
    case DC_CAPTURE_CONT_DOWN_MODE:
        count_down_seconds = MMIDC_GetCountDownSeconds();
        MMIDC_DisplayDelaySecondsTip((uint8)count_down_seconds);
        break;
    case DV_RECORD_MODE:
        MMIDC_DisplayVideoRecordTip(DV_RECORD_MODE);
        break;
    case DV_REVIEW_MODE:
        MMIDC_DisplaySoftKey(mode);
        MMIDC_DisplayVideoReview();
        break;
    default:
        break;
    }

    DisplayPanelItems(mode, is_show);
    

    
    return is_show;
}

/*****************************************************************************/
//Description :  display cmd buttons image
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayCmdBtns(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    uint32 i = 0;
    
    for (i = MMIDC_PDA_BUTTON_CMD_1_CTRL_ID; i <= MMIDC_PDA_BUTTON_CMD_3_CTRL_ID; i++)
    {
        if (PNULL != MMK_GetCtrlHandleByWin(win_id, i))
        {
            MMK_SendMsg(i, MSG_CTL_PAINT, PNULL);
        }
    }
}
/*****************************************************************************/
//Description :  replace list item
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ReplaceListItem(MMI_CTRL_ID_T ctrl_id, MMIDC_SETTINGS_SET_E settings_type)
{
    BOOLEAN   ret = FALSE;
    uint16  index = 0;    
    uint16  cur_index = 0;       
    MMI_TEXT_ID_T text_id = TXT_NULL;
    const GUILIST_ITEM_T* item_ptr = PNULL;
    GUILIST_ITEM_T item_t = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    
    index = (uint16)settings_type + 1;
    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);

    if((item_ptr == PNULL) || (item_ptr->item_data_ptr == PNULL))
    {
        return ret;
    }
    SCI_MEMCPY(&item_t, item_ptr, sizeof(GUILIST_ITEM_T));
    ClearListRect();
    switch(settings_type)
    {
#if defined(MMI_CAMERA_F_PHOTO_DATE)
        case MMIDC_SETTINGS_SET_PHOTO_DATE:
        {
            cur_index = MMIDC_GetNVPhotoDate();
            if(PHOTO_DATE_ON == cur_index)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                text_id = TXT_ACTIVE_OPERATIVE;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
                text_id = TXT_CLOSE;
            }
        }
        break;
#endif        
        case MMIDC_SETTINGS_SET_ENVIRONMENT:
        {
            cur_index = MMIDC_GetPhotoEnvironment();
            if(ENVIRONMENT_NIGHT == cur_index)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;                        
                text_id = TXT_ACTIVE_OPERATIVE;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;                        
                text_id = TXT_CLOSE;
            }
        }
        break;

        case MMIDC_SETTINGS_SET_SHUTTERVOICE:
        {
            cur_index = MMIDC_GetShutterVoice();
            if(SHUTTER_VOICE_1 == cur_index)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;                    
                text_id = TXT_ACTIVE_OPERATIVE;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
                text_id = TXT_CLOSE;
            }
        }
        break;

        case MMIDC_SETTINGS_SET_AUTO_SAVE:
        {
            cur_index = MMIDC_GetIsAutoSave();
            if(AUTO_SAVE_ON == cur_index)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;                    
                text_id = TXT_ACTIVE_OPERATIVE;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
                text_id = TXT_CLOSE;
            }
        }
        break;
        
        default:
        break;
    }
    
    item_t.item_data_ptr->item_content[1].item_data.image_id = image_id;
    item_t.item_data_ptr->item_content[2].item_data.text_id = text_id;
    ret = GUILIST_ReplaceItem(ctrl_id, &item_t, index);        
#if !defined(MMIDC_F_PDA_BIT_16)
    GUILIST_SetBgImage(ctrl_id, IMG_ICONLIST_TEXT_BG, FALSE);
#endif    

    return ret;
}

/*****************************************************************************/
//Description :  MMIDC_SetDCIconSettingList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_SetDCIconSettingList(MMI_HANDLE_T ctrl_id) 
{    
    BOOLEAN     ret = TRUE;
    uint16      index = 0;
    uint32      user_data = 0;
    MMIFILE_DEVICE_E  dev_list[MMI_DEVICE_NUM] = {0};
    int8 max_dev_number = GetStorageDeviceList(dev_list, PNULL);
    //CAMERA_MODE_E   mode = MMIDC_GetCameraMode();   
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    GUILIST_GetItemData(ctrl_id, index, &user_data);

    switch((MMIDC_SETTINGS_SET_E)user_data)
    {
#ifdef MMIDC_F_FLASH
    case MMIDC_SETTINGS_SET_DC_FLASH:
        {
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_FLASH);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
    case MMIDC_SETTINGS_SET_DV_FLASH:
        {
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DV_FLASH);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
#endif
    case MMIDC_SETTINGS_SET_QUALITY:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_QUALITY);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
    case MMIDC_SETTINGS_SET_PHOTO_SIZE:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_SIZE);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
            ResetZoomBtn();
        }
        break;
        
#if defined(MMI_CAMERA_F_PHOTO_DATE)
    case MMIDC_SETTINGS_SET_PHOTO_DATE:
        {
            if(PHOTO_DATE_ON == MMIDC_GetNVPhotoDate())
            {
                MMIDC_SetNVPhotoDate(PHOTO_DATE_OFF);
            }
            else
            {
                MMIDC_SetNVPhotoDate(PHOTO_DATE_ON);
            }
            ret = ReplaceListItem(ctrl_id, (MMIDC_SETTINGS_SET_E)user_data);
            ret = FALSE;
            //MMIDC_CreateListCtrl(ctrl_id, PDA_ICON_SETTING);
        }
        break;
#endif
    case MMIDC_SETTINGS_SET_ENVIRONMENT:
        {
            if(ENVIRONMENT_NIGHT == MMIDC_GetPhotoEnvironment())
            {
                MMIDC_SetPhotoEnvironment(ENVIRONMENT_AUTO);
            }
            else
            {
                MMIDC_SetPhotoEnvironment(ENVIRONMENT_NIGHT);
            }                
            ret = ReplaceListItem(ctrl_id, (MMIDC_SETTINGS_SET_E)user_data);
            ret = FALSE;
            //MMIDC_CreateListCtrl(ctrl_id, PDA_ICON_SETTING);
        }
        break;
        
    case MMIDC_SETTINGS_SET_PIC_MODE:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_PIC_MODE);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
    case MMIDC_SETTINGS_SET_SELF_SHOT:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_SELF_SHOT);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID); 
        }
        break;
        
    case MMIDC_SETTINGS_SET_DC_CORUSCATE:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_CORUSCATE);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
#if !defined(CMCC_UI_STYLE)               
    case MMIDC_SETTINGS_SET_SHUTTERVOICE:
        {
            if(SHUTTER_VOICE_1 == MMIDC_GetShutterVoice())
            {
                MMIDC_SetNVShutterVoice(SHUTTER_VOICE_DISABLE);
            }
            else
            {
                MMIDC_SetNVShutterVoice(SHUTTER_VOICE_1);
            }                
            ret = ReplaceListItem(ctrl_id, (MMIDC_SETTINGS_SET_E)user_data);
            ret = FALSE;
            //MMIDC_CreateListCtrl(ctrl_id, PDA_ICON_SETTING);
        }
        break;
#endif            
    case MMIDC_SETTINGS_SET_AUTO_SAVE:
        {
            if(AUTO_SAVE_ON == MMIDC_GetIsAutoSave())
            {
                MMIDC_SetAutoSave(AUTO_SAVE_OFF);
            }
            else
            {
                MMIDC_SetAutoSave(AUTO_SAVE_ON);
            }                
            ret = ReplaceListItem(ctrl_id, (MMIDC_SETTINGS_SET_E)user_data);
            ret = FALSE;
            //MMIDC_CreateListCtrl(ctrl_id, PDA_ICON_SETTING);
        }
        break;
        
    case MMIDC_SETTINGS_SET_DC_STORAGE:
        {                
            if(max_dev_number >0)
            {
                MMK_DestroyControl(ctrl_id);
                MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DC_STORAGE);
                MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
            }
        }
        break;
        
    case MMIDC_SETTINGS_SET_VIDEO_SIZE:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DV_SIZE);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
    case MMIDC_SETTINGS_SET_DV_CORUSCATE:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DV_CORUSCATE);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
    case MMIDC_SETTINGS_SET_AUDIO:
        {
            if(AUDIO_SETTING_ON == MMIDC_GetVideoAudioSetting())
            {
                MMIDC_SetVideoAudioSetting(AUDIO_SETTING_OFF);
            }
            else
            {
                MMIDC_SetVideoAudioSetting(AUDIO_SETTING_ON);
            }                
            ret = ReplaceListItem(ctrl_id, (MMIDC_SETTINGS_SET_E)user_data);
            ret = FALSE;
            //MMIDC_CreateListCtrl(ctrl_id, PDA_ICON_SETTING);
        }
        break;
        
    case MMIDC_SETTINGS_SET_VIDEOFORMAT:
        {                
            MMK_DestroyControl(ctrl_id);
            MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DV_VIDEOFORMAT);
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
        }
        break;
        
    case MMIDC_SETTINGS_SET_DV_STORAGE:
        {                
            if(max_dev_number >0)
            {
                MMK_DestroyControl(ctrl_id);
                MMIDC_CreateListCtrl(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID, PDA_ICON_SETTING_DV_STORAGE);
                MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID);
            }
        }
        break;
        
    default:
        break;
    }
        
    return (ret);
}

/*****************************************************************************/
//Description : start zoom seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartZoomSeekBarTimer(void)
{

    if (0 == s_zoom_seekbar_info.timer_id)
    {
        s_zoom_seekbar_info.timer_id = MMK_CreateTimer(PDA_SEEKBAR_TIMER, FALSE);
    }

}

/*****************************************************************************/
//Description : stop zoom seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopZoomSeekBarTimer(void)
{
    uint16 timer_id = s_zoom_seekbar_info.timer_id;
    
    if (0 != timer_id)
    {
        MMK_StopTimer(timer_id);
        s_zoom_seekbar_info.timer_id = 0;
    }
}

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description : start zoom seekbar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartZoomSeekBarDisplayTimer(void)
{

    if (0 == s_zoom_seekbar_info.display_timer_id)
    {
        s_zoom_seekbar_info.display_timer_id = MMK_CreateTimer(PDA_SEEKBAR_DISPLAY_TIMER, FALSE);
    }

}

/*****************************************************************************/
//Description : stop zoom seekbar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopZoomSeekBarDisplayTimer(void)
{
    uint16 timer_id = s_zoom_seekbar_info.display_timer_id;
    
    if (0 != timer_id)
    {
        MMK_StopTimer(timer_id);
        s_zoom_seekbar_info.display_timer_id = 0;
    }
}

/*****************************************************************************/
//Description : is display seek bar in recording
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplaySeekBar(void)
{
    BOOLEAN is_display = s_zoom_seekbar_info.is_display_seekbar;

    SCI_TRACE_LOW("[MMIDC] IsDisplaySeekBar is_display=%d", is_display);
    
    return is_display;
}

/*****************************************************************************/
//Description : set display seek bar flag in recording
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetDisplaySeekBarFlag(BOOLEAN flag)
{
    s_zoom_seekbar_info.is_display_seekbar = flag;
}

/*****************************************************************************/
//Description : clear seek bar rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearSeekBarRect(void)
{
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_RECT_T rect = {0};

    rect.left = zoom_info.seekbar_info.frame.cur_rect.left;
    rect.top = zoom_info.seekbar_info.frame.cur_rect.top;
    rect.right = rect.left + zoom_info.seekbar_info.frame.width - 1;
    rect.bottom = rect.top + zoom_info.seekbar_info.frame.height - 1;

    SetDisplaySeekBarFlag(FALSE);
    UILAYER_ClearRect(&dev_info, rect);
    MMIDC_CompleteDisplayOSD();
}

/*****************************************************************************/
//Description : is point in cmd rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPointInCmdRect(GUI_POINT_T point)
{
    int i = 0;
    GUI_RECT_T rect = {0};
    BOOLEAN ret = FALSE;

    for(i = MMIDC_PDA_BUTTON_CMD_1_CTRL_ID; i <= MMIDC_PDA_BUTTON_CMD_3_CTRL_ID; i++)
    {
        GUIAPICTRL_GetRect(i, &rect);
        if(GUI_PointIsInRect(point, rect))
        {
            ret = TRUE;
            break;
        }
    }

    SCI_TRACE_LOW("[MMIDC] IsPointInCmdRect ret=%d", ret);
    return ret;
    
}

/*****************************************************************************/
//Description : handle other msg to display seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void HandleOtherMsgToDisplaySeekBar(BOOLEAN ret, //in
                                                       MMI_MESSAGE_ID_E msg_id, //in
                                                       void *param //in
                                                       )
{
    BOOLEAN is_in_cmd_rect = FALSE;
    
    if (DV_RECORD_MODE == MMIDC_GetCurrentMode()
        || (DV_PAUSE_MODE == MMIDC_GetCurrentMode())
        )
    {
   
        if (((msg_id >= MSG_KEYDOWN_UP) && (msg_id <= MSG_KEYDOWN_AT))
#ifdef TOUCH_PANEL_SUPPORT 
            || ((msg_id >=MSG_TP_PRESS_DOWN) && (msg_id <= MSG_TP_PRESS_DRAG))
#endif
           )
        {
            
            if(!ret
                && !(msg_id == MSG_APP_OK
                || msg_id == MSG_CTL_OK
                || msg_id == MSG_APP_WEB
                || msg_id == MSG_DC_RETURN_KEY
                || msg_id == MSG_CTL_MIDSK
                || msg_id == MSG_APP_CANCEL
                || msg_id == MSG_CTL_CANCEL)
                )
            {
#ifdef TOUCH_PANEL_SUPPORT 
                if(msg_id == MSG_TP_PRESS_DOWN
                    || msg_id == MSG_TP_PRESS_UP)
                {
                    GUI_POINT_T point = {0};
                    
                    point.x = MMK_GET_TP_X(param);
                    point.y = MMK_GET_TP_Y(param);
                    is_in_cmd_rect = IsPointInCmdRect(point);                                               
                }
#endif
                if(!is_in_cmd_rect)
                {
                    StopZoomSeekBarDisplayTimer();
                    StartZoomSeekBarDisplayTimer();                        
                    if(!IsDisplaySeekBar())
                    {
                        SetDisplaySeekBarFlag(TRUE);
                        DisplayZoomSeekBar();  
                        MMIDC_CompleteDisplayOSD();
                    } 
                }
            }
        }
    }
    else
    {
        StopZoomSeekBarDisplayTimer();
    }    
}
#endif

/*****************************************************************************/
//Description : init picture info cur rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitPictureInfoCurRect(MMIDC_PICTURE_INFO_T *pic_info_ptr //in
                                  )
{
    GUI_RECT_T rect = {0};
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        rect = pic_info_ptr->both_rect.v_rect;
    }
    else
    {
        rect = pic_info_ptr->both_rect.h_rect;
    }
    pic_info_ptr->cur_rect = rect;
}

/*****************************************************************************/
//Description : init seekbar cur rect
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitSeekBarCurRect(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                              )
{
    MMIDC_PICTURE_INFO_T *pic_info_ptr = PNULL;
    
    //frame
    pic_info_ptr = &seekbar_info_ptr->frame;
    InitPictureInfoCurRect(pic_info_ptr);
    
    //bg
    pic_info_ptr = &seekbar_info_ptr->bg;
    InitPictureInfoCurRect(pic_info_ptr);
    
    //mask
    pic_info_ptr = &seekbar_info_ptr->mask;
    InitPictureInfoCurRect(pic_info_ptr);
    
    //scale
    pic_info_ptr = &seekbar_info_ptr->scale;
    InitPictureInfoCurRect(pic_info_ptr);    
}

/*****************************************************************************/
//Description : init zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitZoomSeekBarInfo(BOOLEAN is_hor_draw)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T rect = {0};
    GUI_RECT_T frame_rect = {0};
    
    MMIDC_PICTURE_INFO_T pic_info = {0};
    
    MMI_IMAGE_ID_T img_id = 0;
    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;

    int16 left = 0;
    int16 top = 0;
    uint16 scale_height = 0;

    CAMERA_MODE_E mode = MMIDC_GetCameraMode();


    InitZoomSeekBarParam();
    
    //set frame info
    if (is_hor_draw)
    {
        s_zoom_seekbar_info.seekbar_info.is_hor_draw = TRUE;
        
        img_id = IMG_SEEKBAR_FRAME_V;
        left = ZOOM_SEEKBAR_FRAME_V_X;
        top = ZOOM_SEEKBAR_FRAME_V_Y;
    }
    else
    {
        img_id = IMG_SEEKBAR_FRAME;
        left = ZOOM_SEEKBAR_FRAME_H_X;
        top = ZOOM_SEEKBAR_FRAME_H_Y;
    }
    
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);    
    frame_rect.left = left;
    frame_rect.right = frame_rect.left + width - 1;
    frame_rect.top = top;
    frame_rect.bottom = frame_rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = frame_rect;
    pic_info.both_rect.v_rect = rect;
    pic_info.width = width;
    pic_info.height = height;
    
    s_zoom_seekbar_info.seekbar_info.frame = pic_info;
    
    //set bg
    if (is_hor_draw)
    {
        img_id = IMG_SEEKBAR_BG_V;
        left = frame_rect.left + SEEKBAR_BG_INTERVAL_Y;
        top = frame_rect.top + SEEKBAR_BG_INTERVAL_X;
    }
    else
    {
        img_id = IMG_SEEKBAR_BG;
        left = frame_rect.left + SEEKBAR_BG_INTERVAL_X;
        top = frame_rect.top + SEEKBAR_BG_INTERVAL_Y;
    }
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
    rect.left = left;
    rect.right = rect.left + width - 1;
    rect.top = top;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect;
    pic_info.width = width;
    pic_info.height = height;
    
    s_zoom_seekbar_info.seekbar_info.bg = pic_info;
    
#if !defined(MMIDC_F_PDA_BIT_16)     
    //set mask
    if (is_hor_draw)
    {
        img_id = IMG_SEEKBAR_MARK_V;
    }
    else
    {
        img_id = IMG_SEEKBAR_MARK;
        left = frame_rect.left + SEEKBAR_MASK_INTERVAL_X;
        top = frame_rect.top + SEEKBAR_MASK_INTERVAL_Y;
    }
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
    rect.left = left;
    rect.right = rect.left + width - 1;
    rect.top = top;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect;
    pic_info.width = width;
    pic_info.height = height;
    
    s_zoom_seekbar_info.seekbar_info.mask = pic_info;
#endif
    
    //set scale
    if (is_hor_draw)
    {
        img_id = IMG_SEEKBAR_SCALE_V;
    }
    else
    {
        img_id = IMG_SEEKBAR_SCALE;
    }
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
#if defined(MMIDC_F_PDA_BIT_16)  
    if (is_hor_draw)
    {
        width = s_zoom_seekbar_info.seekbar_info.bg.width * 2;
    }
    else
    {
        height = s_zoom_seekbar_info.seekbar_info.bg.height * 2;
    }   
#endif

    rect.left = 0;
    rect.right = rect.left + width - 1;
    rect.top = 0;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect;
    pic_info.width = width;
    pic_info.height = height;
    
    s_zoom_seekbar_info.seekbar_info.scale = pic_info;

    if (CAMERA_MODE_DC == mode)
    {
        total_value = MMIDC_GetMaxZoomLevel() - 1;
        cur_value = MMIDC_GetPhotoZoomValue();
    }
#if defined(MMIDC_F_DV_ZOOM)    
    else
    {
        total_value = MMIDC_GetDVMaxZoomLevel() - 1;
        cur_value = MMIDC_GetDVZoomValue();
    }
#endif

    s_zoom_seekbar_info.seekbar_info.total_value = total_value;
    s_zoom_seekbar_info.seekbar_info.cur_value = cur_value;


    pic_info = s_zoom_seekbar_info.seekbar_info.scale;


    if (is_hor_draw)
    {
        scale_height = pic_info.width;
    }
    else
    {
        scale_height = pic_info.height;
    } 

    total_step = scale_height / 2 / PDA_SEEKBAR_MAP_UNIT;
    if(0 != total_value)
    {
        cur_step = total_step * cur_value / total_value;
    }
    
    s_zoom_seekbar_info.seekbar_info.cur_step = cur_step;
    s_zoom_seekbar_info.seekbar_info.total_step = total_step;

    
    InitSeekBarCurRect(&s_zoom_seekbar_info.seekbar_info);

#if defined(MMIDC_F_DV_ZOOM)
    {  
    uint16 zoom_seekbar_width = 0;

    zoom_seekbar_width = s_zoom_seekbar_info.seekbar_info.frame.width;
    MMIDC_SetZoomSeekBarWidth(zoom_seekbar_width);
    }
#endif    
}

/*****************************************************************************/
//Description : get zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_ZOOM_SEEKBAR_INFO_T GetZoomSeekBarInfo(void)
{
    MMIDC_ZOOM_SEEKBAR_INFO_T seekbar_info = {0};
    
    seekbar_info = s_zoom_seekbar_info;
    
    return  seekbar_info;
}

/*****************************************************************************/
//Description : set zoom seekbar info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetZoomSeekBarInfo(MMIDC_ZOOM_SEEKBAR_INFO_T *seekbar_info_ptr //in
                              )
{
    s_zoom_seekbar_info = *seekbar_info_ptr;
}

/*****************************************************************************/
//Description : get zoom seekbar tp state
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetZoomSeekBarState(void)
{
    BOOLEAN is_down = FALSE;
    
    is_down = s_zoom_seekbar_info.is_tp_down;
    
    return  is_down;
}

/*****************************************************************************/
//Description : set zoom seekbar state
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetZoomSeekBarState(BOOLEAN is_tp_down)
{
    s_zoom_seekbar_info.is_tp_down = is_tp_down;
}

/*****************************************************************************/
//Description : draw seekbar frame
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DrawSeekBarFrame(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                            )
{
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = 0;
    MMIDC_PICTURE_INFO_T frame = {0};
    
    frame = seekbar_info_ptr->frame;
    img_id = frame.img_id;
    x = frame.cur_rect.left;
    y = frame.cur_rect.top;
    
    MMIDC_DisplayBmp(x, y,img_id);
}

/*****************************************************************************/
//Description : draw seekbar scale
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawSeekBarScale(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                               )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    
    MMIDC_PICTURE_INFO_T bg = {0};
    MMIDC_PICTURE_INFO_T scale = {0};
    MMIDC_PICTURE_INFO_T mask = {0};
    
    int16 cur_value = 0;
    int16 total_value = 0;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    uint16 scale_width = 0;
    uint16 scale_height = 0;
    
    MMI_IMAGE_ID_T scale_id = 0;
    MMI_IMAGE_ID_T bg_id = 0;
#if !defined(MMIDC_F_PDA_BIT_16)    
    MMI_IMAGE_ID_T mask_id = 0;
#endif    
    
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T bg_rect = {0};
    GUI_RECT_T rect = {0};
    int16 unit = 0;
    int16 top_space = 0;
    int16 x = 0;
    int16 y = 0;
    GUI_POINT_T point = {0};
    int16 total_scale_height = 0;

    BOOLEAN is_hor_draw = FALSE;
    
    cur_value = seekbar_info_ptr->cur_step;
    total_value = seekbar_info_ptr->total_step;

    if (total_value <= 0)
    {
        return FALSE;
    }

    is_hor_draw = seekbar_info_ptr->is_hor_draw;
    bg = seekbar_info_ptr->bg;
    scale = seekbar_info_ptr->scale;
    mask = seekbar_info_ptr->mask;
    
    bg_id = bg.img_id;
    bg_width = bg.width;
    bg_height = bg.height;
    
    scale_id = scale.img_id;
    scale_width = scale.width;
    scale_height = scale.height;
    
    //clear bg
    //MMIDC_ClearRect(bg.cur_rect, color);
    
    rect = bg.cur_rect;
    x = rect.left;
    y = rect.top;
    //bg
    MMIDC_DisplayBmp(x, y, bg_id);
    
    /*
    *get intersect rect
    */

    if (is_hor_draw)
    {
        total_scale_height = scale_width / 2;
        
        unit = total_scale_height / total_value;
        top_space = cur_value * unit;
        if ((total_scale_height - top_space) < unit)
        {
            top_space = total_scale_height - SEEKBAR_SCALE_FOCUS_LINE_HEIGHT;
        }
        
        bg_rect.left = total_scale_height - top_space;
        bg_rect.right = bg_rect.left + total_scale_height - 1 + 1;
        bg_rect.top = 0;
        bg_rect.bottom = bg_rect.top + scale_height - 1;
        
        
        disp_rect.left = SEEKBAR_SCALE_CALIBRATE_VALUE;
        disp_rect.right = disp_rect.left + scale_width - 1;
        disp_rect.top = 0;
        disp_rect.bottom = disp_rect.top + scale_height - 1;
    }
    else
    {
        total_scale_height = scale_height / 2;
        
        unit = total_scale_height / total_value;
        top_space = cur_value * unit;
        if ((total_scale_height - top_space) < unit)
        {
            top_space = total_scale_height - SEEKBAR_SCALE_FOCUS_LINE_HEIGHT;
        }
        
        bg_rect.left = 0;
        bg_rect.right = bg_rect.left + scale_width - 1;
        bg_rect.top = top_space;
        bg_rect.bottom = bg_rect.top + total_scale_height - 1 + 1;
        
        
        disp_rect.left = 0;
        disp_rect.right = disp_rect.left + scale_width - 1;
        disp_rect.top = SEEKBAR_SCALE_CALIBRATE_VALUE;
        disp_rect.bottom = disp_rect.top + scale_height - 1;

    }

#if defined(MMIDC_F_PDA_BIT_16)
    GUIRES_GetImgWidthHeight(&scale_width, &scale_height, scale_id, win_id);

    if (is_hor_draw)
    {
        rect = bg.cur_rect;
        disp_rect.left = rect.left + top_space - scale_width / 2 - 1;
        disp_rect.top = rect.top + (bg_height - scale_height) / 2 - 1;

        if (disp_rect.left < rect.left)
        {
            disp_rect.left = rect.left;
        }
        if ((disp_rect.left + scale_width - 1) > rect.right)
        {
            disp_rect.left = rect.right - scale_width + 1;
        }
    }
    else
    {
        rect = bg.cur_rect;
        disp_rect.left = rect.left + (bg_width - scale_width) / 2 - 1;
        disp_rect.top = rect.top + bg_height - top_space - scale_height / 2 - 1;

        if (disp_rect.top < rect.top)
        {
            disp_rect.top = rect.top;
        }
        if ((disp_rect.top + scale_height - 1) > rect.bottom)
        {
            disp_rect.top = rect.bottom - scale_height + 1;
        }
    }

    point.x = disp_rect.left;
    point.y = disp_rect.top;
    GUIRES_DisplayImg(&point,
    PNULL,
    PNULL,
    win_id,
    scale_id,
    &dev_info);
#else
    GUI_IntersectRect(&disp_rect, bg_rect, disp_rect);
    
    rect = bg.cur_rect;
    if (is_hor_draw)
    {
        point.x = rect.left + SEEKBAR_SCALE_INTERVAL_V_SPACE;
        point.y = rect.top + SEEKBAR_SCALE_INTERVAL_H_SPACE;
    }
    else
    {
        point.x = rect.left + SEEKBAR_SCALE_INTERVAL_H_SPACE;
        point.y = rect.top + SEEKBAR_SCALE_INTERVAL_V_SPACE;
    }
    GUIRES_DisplayImg(&point,
        PNULL,
        &disp_rect,
        win_id,
        scale_id,
        &dev_info);
    
    
    //mask    
    mask_id = mask.img_id;
    rect = mask.cur_rect;
    x = rect.left;
    y = rect.top;
    MMIDC_DisplayBmp(x, y, mask_id);
#endif

    return TRUE;
}
                               
/*****************************************************************************/
//Description : draw seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DrawSeekBar(MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                       )
{
    DrawSeekBarFrame(seekbar_info_ptr);    
    DrawSeekBarScale(seekbar_info_ptr);
}

/*****************************************************************************/
//Description : pda update zoom value
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN PDAUpdateZoomValue(int16 cur_value)
{
    BOOLEAN ret = FALSE;
    int32 zoom_max = 0;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();


    if (CAMERA_MODE_DC == mode)
    {
        zoom_max = MMIDC_GetMaxZoomLevel();
    }
#if defined(MMIDC_F_DV_ZOOM)    
    else
    {
        zoom_max = MMIDC_GetDVMaxZoomLevel();
    }
#endif    
    
    if(!MMIDC_IsReviewing() && !MMIDC_IsCapturing() && !MMIDC_IsSettingParamCtrl())
    {
        if ((0 <= cur_value) && (cur_value < zoom_max))
        {
            ret = TRUE;

            if (CAMERA_MODE_DC == mode)
            {
                MMIDC_UpdatePhotoZoomValue(cur_value);
            }
#if defined(MMIDC_F_DV_ZOOM)            
            else
            {
                MMIDC_UpdateDVZoomValue(cur_value);
            }
#endif            
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] PDAUpdateZoomValue ret=%d, cur_value=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7082_112_2_18_2_9_6_290,(uint8*)"dd",ret, cur_value);
    return ret;
}

/*****************************************************************************/
//Description : update zoom value
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateZoomValue(void)
{
    BOOLEAN ret = FALSE;

    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    

    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
    uint16 aveg_space = 0;
    
    seekbar_info = zoom_info.seekbar_info;
    
    total_value = seekbar_info.total_value;
    total_step = seekbar_info.total_step;
    cur_step = seekbar_info.cur_step;

    cur_value = total_value * cur_step / total_step;        

    //adjust cur_value
    aveg_space = total_step / total_value;

    if( (cur_step - (cur_value * aveg_space)) > (aveg_space / 2)
        && cur_value < total_value)
    {
        cur_value++;
    }        
        
    seekbar_info.cur_value = cur_value;
    zoom_info.seekbar_info = seekbar_info;
        
    PDAUpdateZoomValue(cur_value);
    //SetZoomSeekBarInfo(&zoom_info);

    ret = TRUE;
    
    //SCI_TRACE_LOW:"[MMIDC] UpdateZoomValue cur_step=%d,total_step=%d,total_value=%d,cur_value=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7120_112_2_18_2_9_6_291,(uint8*)"dddd",cur_step, total_step,total_value, cur_value);

    return ret;
}

/*****************************************************************************/
//Description : handle zoom seek bar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTimer(uint16 timer_id)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    uint16 zoom_timer = 0;
    
    
    zoom_timer = zoom_info.timer_id;
    if (zoom_timer == timer_id)
    {
        ret = UpdateZoomValue();
        
        StopZoomSeekBarTimer();
        StartZoomSeekBarTimer();        
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTimer ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7147_112_2_18_2_9_6_292,(uint8*)"d",ret);
    
    return ret;
}

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description : handle zoom seek bar display timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarDisplayTimer(uint16 timer_id)
{
    BOOLEAN ret = FALSE;    
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    uint16 display_zoom_timer = 0;    
    BOOLEAN is_down = GetZoomSeekBarState();
    
    display_zoom_timer = zoom_info.display_timer_id;
    if(display_zoom_timer == timer_id)
    {
        DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
        
        ret = TRUE;
        StopZoomSeekBarDisplayTimer();
        if(DV_RECORD_MODE == mode || DV_PAUSE_MODE == mode)
        {
            if(is_down)
            {
                StartZoomSeekBarDisplayTimer();
            }
            else
            {
                ClearSeekBarRect();
            }
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] HandleZoomSeekBarDisplayTimer ret=%d", ret);
    
    return ret;
}
#endif

/*****************************************************************************/
//Description : handle zoom seek bar tp move
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpMove(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_PICTURE_INFO_T bg = {0};
    MMIDC_PICTURE_INFO_T scale = {0};
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    
    GUI_POINT_T down_point = {0};
    GUI_RECT_T rect = {0};
    int16 cur_value = 0;
    int16 total_value = 0;
    int16 y_step = 0;
    int16 total_height = 0;
    int16 unit = 0;
    int16 move_value = 0;
    BOOLEAN is_tp_down = FALSE;
    BOOLEAN is_hor_draw = FALSE;
#if defined(MMIDC_F_DV_ZOOM)
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();

    if( DC_CAPTURE_CONT_DOWN_MODE == flow_mode
		|| DC_CAPTURE_MODE == flow_mode
		|| DC_SAVE_MODE == flow_mode
		|| DC_REVIEW_MODE == flow_mode
		||DV_REVIEW_MODE == flow_mode)
    {
        return ret;
    }
#endif
    
    is_tp_down = zoom_info.is_tp_down;
    down_point = zoom_info.down_point;
    seekbar_info = zoom_info.seekbar_info;

    is_hor_draw = seekbar_info.is_hor_draw;

    scale = seekbar_info.scale;
    bg = seekbar_info.bg;
    rect = bg.cur_rect;

    
    if (point.y != down_point.y
        && GUI_PointIsInRect(point, rect)
        && is_tp_down
        )
    {
        if (is_hor_draw)
        {
            y_step = point.x - down_point.x;
            total_height = scale.width / 2;
        }
        else
        {
            y_step = down_point.y - point.y;
            total_height = scale.height / 2;
        }
        
        total_value = seekbar_info.total_step; 
        cur_value = seekbar_info.cur_step;

        if (0 < total_value)
        {               

            unit = total_height / total_value;      
            if (0 < unit)
            {
                if (y_step >= 0)
                {
                    move_value = (y_step + unit - 1) / unit;
                }
                else
                {
                    move_value = (y_step - unit + 1) / unit;
                }                

                
                cur_value = cur_value + move_value;
                
                if (cur_value > total_value)
                {
                    cur_value = total_value;
                }
                
                if (cur_value < 0)
                {
                    cur_value = 0;
                }
                seekbar_info.cur_step = cur_value;
                
                zoom_info.seekbar_info = seekbar_info;
                zoom_info.down_point = point;
                
                SetZoomSeekBarInfo(&zoom_info);
                
                DrawSeekBarScale(&seekbar_info); 
                
                MMIDC_CompleteDisplayOSD();      

                ret = TRUE;
            }

        }

        //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTPMove y_step=%d,move_value=%d,cur_value=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7250_112_2_18_2_9_7_293,(uint8*)"ddd",y_step, move_value, cur_value);

    }
        
    
    //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTPMove ret=%d,is_down=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7256_112_2_18_2_9_7_294,(uint8*)"dd",ret, is_tp_down);
    
    return ret;
}

/*****************************************************************************/
//Description : handle zoom seek bar tp down
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpDown(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    GUI_RECT_T rect = {0};
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    BOOLEAN is_zoom_btn_press = FALSE;
    
#if defined(MMIDC_F_DV_ZOOM)
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();
    uint32 max_zoom = MMIDC_GetDVMaxZoomLevel();

    if(!IsDisplaySeekBar()
        && (DV_RECORD_MODE == flow_mode
        || DV_PAUSE_MODE == flow_mode))
    {
        return ret;
    }
    if ((DV_RECORD_MODE == flow_mode)
        || (DV_PAUSE_MODE == flow_mode)
        )
    {
        if (max_zoom > 1)
        {
            is_zoom_btn_press = TRUE;
        }
    }
    else
#endif  
    {
        is_zoom_btn_press = GetIconStateAsPress(PDA_RECT_ZOOM);
    }
    
    seekbar_info = zoom_info.seekbar_info;
    rect = seekbar_info.bg.cur_rect;
    if (
        GUI_PointIsInRect(point, rect) 
        && is_zoom_btn_press
        )
    {
        ret = TRUE;
        
        zoom_info.down_point = point;
        zoom_info.is_tp_down = TRUE;
        
        SetZoomSeekBarInfo(&zoom_info);

#ifdef MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION        
        StopZoomSeekBarTimer();
        StartZoomSeekBarTimer();
#endif
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTpDown ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7314_112_2_18_2_9_7_295,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//Description : handle zoom seek bar tp up
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomSeekBarTpUp(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;

   
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
#if 0
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
#endif

    BOOLEAN is_down = GetZoomSeekBarState();
    
#if defined(MMIDC_F_DV_ZOOM)
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();

    if( DC_CAPTURE_CONT_DOWN_MODE == flow_mode
		|| DC_CAPTURE_MODE == flow_mode
		|| DC_SAVE_MODE == flow_mode
		|| DC_REVIEW_MODE == flow_mode
		|| DV_REVIEW_MODE == flow_mode)
    {
        return ret;
    }
#endif  
    
    if (is_down)
    {
        ret = TRUE;

#ifdef MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION        
        StopZoomSeekBarTimer();
#else
        {
            BOOLEAN update_result = FALSE;
            
            update_result = UpdateZoomValue();
            
            //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTpUp update_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7353_112_2_18_2_9_7_296,(uint8*)"d",update_result);

            zoom_info = GetZoomSeekBarInfo();
        }
#endif
        
        zoom_info.is_tp_down = FALSE;


        seekbar_info = zoom_info.seekbar_info;
#if 0
        if (CAMERA_MODE_DC == mode)
        {
            seekbar_info.cur_value = MMIDC_GetPhotoZoomValue();
        }
#if defined(MMIDC_F_DV_ZOOM)        
        else
        {
            seekbar_info.cur_value = MMIDC_GetDVZoomValue();
        }
#endif        
        
        cur_value = seekbar_info.cur_value;
        total_value = seekbar_info.total_value;    
        total_step = seekbar_info.total_step;
        cur_step = total_step * cur_value / total_value;

        seekbar_info.cur_step = cur_step;        
#endif     

        zoom_info.seekbar_info = seekbar_info;
        
        SetZoomSeekBarInfo(&zoom_info);
        
        //repaint seekbar
        DrawSeekBarScale(&seekbar_info); 
        MMIDC_CompleteDisplayOSD();
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleZoomSeekBarTpUp ret=%d,is_down=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7393_112_2_18_2_9_7_297,(uint8*)"dd",ret, is_down);
    
    return ret;
}

/*****************************************************************************/
//Description : display zoom seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayZoomSeekBar(void)
{
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    
    DrawSeekBar(&zoom_info.seekbar_info);  
}

/*****************************************************************************/
//Description : create image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateImageAttrInfo(void)
{
    BOOLEAN ret = FALSE;
    
    if (PNULL == s_image_attr_info_ptr)
    {
        s_image_attr_info_ptr = (MMIDC_IMAGE_ATTR_INFO_T *)SCI_ALLOCA(sizeof(MMIDC_IMAGE_ATTR_INFO_T));
        
        if (PNULL != s_image_attr_info_ptr)
        {
            ret = TRUE;
            
            SCI_MEMSET(s_image_attr_info_ptr, 0x00, sizeof(*s_image_attr_info_ptr));
        }
        
        //SCI_TRACE_LOW:"[MMIDC] CreateImageAttrInfo attr_ptr=0x%lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7429_112_2_18_2_9_7_298,(uint8*)"d",s_image_attr_info_ptr);
    }
    else
    {
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] CreateImageAttrInfo ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7437_112_2_18_2_9_7_299,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//Description : destroy image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DestroyImageAttrInfo(void)
{
    //SCI_TRACE_LOW:"[MMIDC] DestroyImageAttrInfo attr_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7449_112_2_18_2_9_7_300,(uint8*)"d",s_image_attr_info_ptr);
    
    if (PNULL != s_image_attr_info_ptr)
    {
        SCI_FREE(s_image_attr_info_ptr);
    }
}

/*****************************************************************************/
//Description : get image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_IMAGE_ATTR_INFO_T* GetImageAttrInfoByPtr(void)
{
    MMIDC_IMAGE_ATTR_INFO_T* attr_ptr = PNULL;
    
    attr_ptr = s_image_attr_info_ptr;
    
    return attr_ptr;
}

/*****************************************************************************/
//Description : set image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetImageAttrInfo(MMIDC_IMAGE_ATTR_INFO_T* attr_info)
{
    MMIDC_IMAGE_ATTR_INFO_T* attr_ptr = PNULL;
    
    attr_ptr = s_image_attr_info_ptr;
    
    if (PNULL != attr_ptr)
    {
        *attr_ptr = *attr_info;
    }
}

/*****************************************************************************/
//Description : init image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitImageAttrSeekBar(int16 x, //in
                                int16 y, //in
                                MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr //in
                                )
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T rect = {0};
    GUI_RECT_T frame_rect = {0};
    
    MMIDC_PICTURE_INFO_T pic_info = {0};
    MMI_IMAGE_ID_T img_id = 0;

    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
    
    /*
    //set frame info
    img_id = IMG_SEEKBAR_FRAME;
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
    frame_rect.left = ZOOM_SEEKBAR_FRAME_H_X;
    frame_rect.right = frame_rect.left + width - 1;
    frame_rect.top = ZOOM_SEEKBAR_FRAME_H_Y;
    frame_rect.bottom = frame_rect.top + height -1;
    
      pic_info.img_id = img_id;
      pic_info.both_rect.h_rect = frame_rect;
      pic_info.width = width;
      pic_info.height = height;
      
        s_zoom_seekbar_info.frame = pic_info;
    */
    
    frame_rect.left = x;
    frame_rect.top = y;
    
    //set bg
    img_id = IMG_SEEKBAR_BG;
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
    rect.left = frame_rect.left + SEEKBAR_BG_INTERVAL_X;
    rect.right = rect.left + width - 1;
    rect.top = frame_rect.top;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect; 
    pic_info.width = width;
    pic_info.height = height;
    
    seekbar_info_ptr->bg = pic_info;
    
#if !defined(MMIDC_F_PDA_BIT_16)    
    //set mask
    img_id = IMG_SEEKBAR_MARK;
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
    rect.left = frame_rect.left + SEEKBAR_MASK_INTERVAL_X;
    rect.right = rect.left + width - 1;
    rect.top = frame_rect.top;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect;     
    pic_info.width = width;
    pic_info.height = height;
    
    seekbar_info_ptr->mask = pic_info;
#endif    
    
    //set scale
    img_id = IMG_SEEKBAR_SCALE;
    GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

#if defined(MMIDC_F_PDA_BIT_16)
    height = seekbar_info_ptr->bg.height * 2;
#endif    

    
    rect.left = 0;
    rect.right = rect.left + width - 1;
    rect.top = 0;
    rect.bottom = rect.top + height -1;
    
    pic_info.img_id = img_id;
    pic_info.both_rect.h_rect = rect;
    pic_info.both_rect.v_rect = rect; 
    pic_info.width = width;
    pic_info.height = height;
    
    seekbar_info_ptr->scale = pic_info;

    cur_value = seekbar_info_ptr->cur_value;
    total_value = seekbar_info_ptr->total_value;  
    total_step = pic_info.height / 2 / PDA_SEEKBAR_MAP_UNIT;
    cur_step = total_step * cur_value / total_value;

    seekbar_info_ptr->cur_step = cur_step;
    seekbar_info_ptr->total_step = total_step;
}        

/*****************************************************************************/
//Description : init image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitImageAttrInfo(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr = PNULL;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    
    uint16 total = 0;
    uint16 i = 0;
    int16 x = 0;
    int16 y = 0;
    
    if (PNULL != attr_ptr)
    {
        if(CAMERA_MODE_DC == mode)
        {
            total = 3;
            attr_ptr->total_num = total;
            for (i = 0 ; i < total; i++)
            {
                seekbar_info_ptr = &attr_ptr->attr[i];
                
                seekbar_info_ptr->total_value = (BRIGHTNESS_LEVEL_MAX - 1);
                switch(i)
                {
                case IMAGE_ATTR_BRIGHTNESS: 
                    if (SCREEN_MODE_VERTICAL == screen_mode)
                    {
                        x = BRIGHTNESS_SEEKBAR_FRAME_V_X;
                        y = BRIGHTNESS_SEEKBAR_FRAME_V_Y;
                    }
                    else
                    {
                        x = BRIGHTNESS_SEEKBAR_FRAME_H_X;
                        y = BRIGHTNESS_SEEKBAR_FRAME_H_Y;
                    }
                    seekbar_info_ptr->cur_value = MMIDC_GetPhotoBrightness();
                    break;  
                case IMAGE_ATTR_CONTRAST:    
                    if (SCREEN_MODE_VERTICAL == screen_mode)
                    {
                        x = CONTRAST_SEEKBAR_FRAME_V_X;
                        y = CONTRAST_SEEKBAR_FRAME_V_Y;
                    }
                    else
                    {
                        x = CONTRAST_SEEKBAR_FRAME_H_X;
                        y = CONTRAST_SEEKBAR_FRAME_H_Y;
                    }
                    seekbar_info_ptr->cur_value = MMIDC_GetContrast();
                    break;  
                case IMAGE_ATTR_EXPOSALCOMPENSATE:  
                    if (SCREEN_MODE_VERTICAL == screen_mode)
                    {
                        x = EC_SEEKBAR_FRAME_V_X;
                        y = EC_SEEKBAR_FRAME_V_Y;
                    }
                    else
                    {
                        x = EC_SEEKBAR_FRAME_H_X;
                        y = EC_SEEKBAR_FRAME_H_Y;
                    }
                    seekbar_info_ptr->cur_value = MMIDC_GetExposalCompensate();
                    break;      
                default:
                    break;
                }
                
                InitImageAttrSeekBar(x, y, seekbar_info_ptr);
                InitSeekBarCurRect(seekbar_info_ptr);
            }
        }
        else
        {
            total = 2;
            attr_ptr->total_num = total;
            for (i = 0 ; i < total; i++)
            {
                seekbar_info_ptr = &attr_ptr->attr[i];
                
                seekbar_info_ptr->total_value = (BRIGHTNESS_LEVEL_MAX - 1);
                switch(i)
                {
                case IMAGE_ATTR_BRIGHTNESS:   
                    if (SCREEN_MODE_VERTICAL == screen_mode)
                    {
                        x = DV_BRIGHTNESS_SEEKBAR_FRAME_V_X;
                        y = DV_BRIGHTNESS_SEEKBAR_FRAME_V_Y;
                    }
                    else
                    {
                        x = DV_BRIGHTNESS_SEEKBAR_FRAME_H_X;
                        y = DV_BRIGHTNESS_SEEKBAR_FRAME_H_Y;
                    }
                    seekbar_info_ptr->cur_value = MMIDC_GetVideoBrightness();
                    break;  
                case IMAGE_ATTR_CONTRAST:
                    if (SCREEN_MODE_VERTICAL == screen_mode)
                    {
                        x = DV_CONTRAST_SEEKBAR_FRAME_V_X;
                        y = DV_CONTRAST_SEEKBAR_FRAME_V_Y;
                    }
                    else
                    {
                        x = DV_CONTRAST_SEEKBAR_FRAME_H_X;
                        y = DV_CONTRAST_SEEKBAR_FRAME_H_Y;
                    }
                    seekbar_info_ptr->cur_value = MMIDC_GetVideoContrast();
                    break;  
                default:
                    break;
                }
                
                InitImageAttrSeekBar(x, y, seekbar_info_ptr);
                InitSeekBarCurRect(seekbar_info_ptr);
            }
        }
    }
}

/*****************************************************************************/
//Description : reset image attributes info
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetImageAttrInfo(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T *seekbar_info_ptr = PNULL;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    uint16 total = 0;
    uint16 i = 0;
    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
    
    if (PNULL != attr_ptr)
    {
        total = attr_ptr->total_num;
        for (i = 0 ; i < total; i++)
        {
            seekbar_info_ptr = &attr_ptr->attr[i];
            
            switch(i)
            {
            case IMAGE_ATTR_BRIGHTNESS: 
                if (CAMERA_MODE_DC == mode)
                {
                    cur_value = MMIDC_GetPhotoBrightness();
                }
                else
                {
                    cur_value = MMIDC_GetVideoBrightness();
                }
                break;  
            case IMAGE_ATTR_CONTRAST:
                if (CAMERA_MODE_DC == mode)
                {
                    cur_value = MMIDC_GetContrast();
                }
                else
                {
                    cur_value = MMIDC_GetVideoContrast();
                }
                break;  
            case IMAGE_ATTR_EXPOSALCOMPENSATE:  
                cur_value = MMIDC_GetExposalCompensate();
                break;      
            default:
                break;
            }
            seekbar_info_ptr->cur_value = cur_value;
            
            total_value = seekbar_info_ptr->total_value;    
            total_step = seekbar_info_ptr->total_step;
            cur_step = total_step * cur_value / total_value;
            
            seekbar_info_ptr->cur_step = cur_step;
        }
    }
}

/*****************************************************************************/
//Description : start image attribute seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartImageAttrSeekBarTimer(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    
    if (PNULL != attr_ptr)
    {
        if (0 == attr_ptr->timer_id)
        {
            attr_ptr->timer_id = MMK_CreateTimer(PDA_SEEKBAR_TIMER, FALSE);
        }
    }
}

/*****************************************************************************/
//Description : stop image attribute seekbar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StopImageAttrSeekBarTimer(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    uint16 timer_id = 0;
    
    if (PNULL != attr_ptr)
    {
        timer_id = attr_ptr->timer_id;
        if (0 != timer_id)
        {
            MMK_StopTimer(timer_id);
            attr_ptr->timer_id = 0;
        }
    }    
}

/*****************************************************************************/
//Description : get image attribute seekbar tp state
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImageAttrSeekBarState(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    BOOLEAN is_down = FALSE;
    
    if (PNULL != attr_ptr)
    {
        is_down = attr_ptr->is_tp_down;
    }
    
    return  is_down;
}

/*****************************************************************************/
//Description : set seekbar state
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetImageAttrSeekBarState(BOOLEAN is_tp_down)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    
    if (PNULL != attr_ptr)
    {
        attr_ptr->is_tp_down = is_tp_down;
    }
}
/*****************************************************************************/
//Description : pda update image attribute value
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN PDAUpdateImageAttrValue(IMAGE_ATTR_E cur_attr,
                                      int16 cur_value
                                      )
{
    BOOLEAN ret = FALSE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();

    if(!MMIDC_IsReviewing() && !MMIDC_IsCapturing() && !MMIDC_IsSettingParamCtrl())
    {
        if ((0 <= cur_value)
            && (cur_value <= BRIGHTNESS_LEVEL_MAX - 1)
           )
        {
            switch(cur_attr)
            {
            case IMAGE_ATTR_BRIGHTNESS:
                if (CAMERA_MODE_DC == mode)
                {
                    MMIDC_SetPhotoBrightness((BRIGHTNESS_E)cur_value);
                }
                else
                {
                    MMIDC_SetVideoBrightness((BRIGHTNESS_E)cur_value);
                }
                break;
            case IMAGE_ATTR_CONTRAST:
                if (CAMERA_MODE_DC == mode)
                {
                    MMIDC_SetContrast((CONTRAST_E)cur_value);
                }
                else
                {
                    MMIDC_SetVideoContrast((CONTRAST_E)cur_value);
                }
                break;
            case IMAGE_ATTR_EXPOSALCOMPENSATE:
                MMIDC_SetExposalCompensate((EXPOSAL_COMPENSATE_E)cur_value);
                break;
            default:
                break;
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIDC] PDAUpdateImageAttrValue ret=%d, cur_value=%d,cur_attr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7897_112_2_18_2_9_8_301,(uint8*)"ddd",ret, cur_value,cur_attr);
        
    return ret;
}

/*****************************************************************************/
//Description : update image attribute value
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateImageAttrValue(void)
{
    BOOLEAN ret = FALSE;

    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};

    IMAGE_ATTR_E cur_attr = 0;        

    int16 cur_value = 0;
    int16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
	uint16 step_uint = 0;


    if (PNULL != attr_ptr)
    {
        ret = TRUE;
        
        //get current seekbar info
        cur_attr = attr_ptr->cur_attr;
        seekbar_info = attr_ptr->attr[cur_attr]; 

        total_value = seekbar_info.total_value;
        total_step = seekbar_info.total_step;
        cur_step = seekbar_info.cur_step;

		step_uint = total_step / total_value;
		cur_value = cur_step / step_uint;
        cur_value = ((cur_step % step_uint) > (step_uint / 2))?(cur_value + 1):cur_value;
            
        seekbar_info.cur_step  = cur_value * total_step / total_value;
        seekbar_info.cur_value = cur_value;
        attr_ptr->attr[cur_attr]= seekbar_info;
        
        
        //update image attribute value
        PDAUpdateImageAttrValue(cur_attr,cur_value);

        
        //SCI_TRACE_LOW:"[MMIDC] UpdateImageAttrValue cur_step=%d,total_step =%d,total_value=%d,cur_value=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7944_112_2_18_2_9_8_302,(uint8*)"dddd",cur_step, total_step, total_value, cur_value);
    }
    
    return ret;
}
/*****************************************************************************/
//Description : handle image attribute seek bar timer
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTimer(uint16 timer_id)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    
    uint16 timer = 0;
    
    if (PNULL != attr_ptr)
    {
        timer = attr_ptr->timer_id;
        if (timer == timer_id)
        {   
            ret = UpdateImageAttrValue();
            
            StopImageAttrSeekBarTimer();
            StartImageAttrSeekBarTimer();
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTimer ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_7974_112_2_18_2_9_8_303,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//Description : handle image attribute seek bar tp move
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpMove(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    MMIDC_PICTURE_INFO_T bg = {0};
    MMIDC_PICTURE_INFO_T scale = {0};
    IMAGE_ATTR_E cur_attr = 0;    
    
    
    GUI_POINT_T down_point = {0};
    GUI_RECT_T rect = {0};
    int16 cur_value = 0;
    int16 total_value = 0;
    int16 y_step = 0;
    int16 total_height = 0;
    int16 unit = 0;
    int16 move_value = 0;

    BOOLEAN is_down = FALSE;
    
    if (PNULL != attr_ptr)
    {
        is_down = attr_ptr->is_tp_down;

        //get current seekbar info
        cur_attr = attr_ptr->cur_attr;
        down_point = attr_ptr->down_point;
        seekbar_info = attr_ptr->attr[cur_attr]; 

        scale = seekbar_info.scale;
        bg = seekbar_info.bg;
        rect = bg.cur_rect;        
       
        if (point.y != down_point.y
            && GUI_PointIsInRect(point, rect)
            && is_down
            )
        {
            ret = TRUE;
            
            y_step = down_point.y - point.y;  
            total_value = seekbar_info.total_step; 
            cur_value = seekbar_info.cur_step;
            total_height = scale.height / 2;
            
            unit = total_height / total_value;  
            if (y_step >= 0)
            {
                move_value = (y_step + unit - 1) / unit;
            }
            else
            {
                move_value = (y_step - unit + 1) / unit;
            }
            
            cur_value = cur_value + move_value;
            
            if (cur_value > total_value)
            {
                cur_value = total_value;
            }
            
            if (cur_value < 0)
            {
                cur_value = 0;
            }
            seekbar_info.cur_step = cur_value;
            
            attr_ptr->down_point = point;
            attr_ptr->attr[cur_attr]= seekbar_info;
            
            //repaint seekbar
            DrawSeekBarScale(&seekbar_info); 
            MMIDC_CompleteDisplayOSD();

            //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpMove y_step=%d,move_value=%d,cur_value=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8061_112_2_18_2_9_8_304,(uint8*)"ddd",y_step, move_value, cur_value);
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpMove ret=%d,is_down=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8066_112_2_18_2_9_8_305,(uint8*)"dd",ret, is_down);
    
    return ret;
}

/*****************************************************************************/
//Description : handle image attribute seek bar tp down
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpDown(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    
    uint16 i = 0;
    uint16 total = 0;
    GUI_RECT_T rect = {0};
    BOOLEAN is_attr_press = FALSE;

    if (PNULL != attr_ptr)
    {
        is_attr_press = GetIconStateAsPress(PDA_RECT_IMAGE_PROPERTY);

        total = attr_ptr->total_num;
        for (i = 0; i < total; i++)
        {
            seekbar_info = attr_ptr->attr[i];
            rect = seekbar_info.bg.cur_rect;
            
            if (
                GUI_PointIsInRect(point, rect)
                && is_attr_press
                )
            {
                ret = TRUE;
                
                attr_ptr->cur_attr = (IMAGE_ATTR_E)i;
                attr_ptr->down_point = point;
                attr_ptr->is_tp_down = TRUE;
#ifdef MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION
                StopImageAttrSeekBarTimer();
                StartImageAttrSeekBarTimer();
#endif

                break;
            }    
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpDown ret=%d, is_attr_press=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8118_112_2_18_2_9_8_306,(uint8*)"dd",ret, is_attr_press);
    
    return ret;
}

/*****************************************************************************/
//Description : handle image attribute seek bar tp up
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleImageAttrSeekBarTpUp(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};

    IMAGE_ATTR_E cur_attr = 0;
    BOOLEAN is_down = FALSE;
    
    uint16 cur_value = 0;
    uint16 total_value = 0;
    uint16 cur_step = 0;
    uint16 total_step = 0;
    
    if (PNULL != attr_ptr)
    {
        is_down = attr_ptr->is_tp_down;
        
        if (is_down)
        {
            ret = TRUE;

#ifdef MMIDC_PDA_SEEKBAR_SCROLL_SET_FUNCTION
            StopImageAttrSeekBarTimer();
#else
        {
            BOOLEAN update_result = FALSE;
            
            update_result = UpdateImageAttrValue();
            
            //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpUp update_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8159_112_2_18_2_9_8_307,(uint8*)"d",update_result);
        }
#endif
            
            //ResetImageAttrInfo();
            
            attr_ptr->is_tp_down = FALSE;;

            cur_attr = attr_ptr->cur_attr;
            seekbar_info = attr_ptr->attr[cur_attr];
            /*
            cur_value = seekbar_info.cur_value;
            total_value = seekbar_info.total_value;    
            total_step = seekbar_info.total_step;
            cur_step = total_step * cur_value / total_value;

            seekbar_info.cur_step = cur_step;
            attr_ptr->attr[cur_attr]= seekbar_info;
            */
            
            //repaint seekbar
            DrawSeekBarScale(&seekbar_info); 
            MMIDC_CompleteDisplayOSD();

            //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpUp cur_value=%d,total_value=%d,cur_step=%d,total_step"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8184_112_2_18_2_9_9_308,(uint8*)"ddd",cur_value, total_value, cur_step, total_step);

        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] HandleImageAttrSeekBarTpUp ret=%d,point.x=%d,pointl.y=%d, is_down=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8190_112_2_18_2_9_9_309,(uint8*)"dddd",ret, point.x, point.y, is_down);
    
    return ret;
}

/*****************************************************************************/
//Description : display image attribute seek bar
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayImageAttrAllSeekBar(void)
{
    MMIDC_IMAGE_ATTR_INFO_T *attr_ptr = GetImageAttrInfoByPtr();
    MMIDC_SEEKBAR_INFO_T seekbar_info = {0};
    uint16 i = 0;
    uint16 total = 0;
    
    DrawPicPropertyBackground();  
    CreateLabel(MMIDC_PDA_LABEL_CTRL_ID);    
    DrawPicPropertyBmp();    
    DrawBrightnessBmp();
    DrawContrastBmp();
    DrawEcBmp();
    
    if (PNULL != attr_ptr)
    {
        total = attr_ptr->total_num;
        for (i = 0; i < total; i++)
        {
            seekbar_info = attr_ptr->attr[i];
            
            DrawSeekBar(&seekbar_info); 
        }
    }
}

/*****************************************************************************/
//Description : set menu key state
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetMenuKeyState(PDA_MENU_KEY_STATE_E  key_state)
{
    s_menu_key_state = key_state;
}     

/*****************************************************************************/
//Description : get menu key state
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL PDA_MENU_KEY_STATE_E GetMenuKeyState(void)
{
    return s_menu_key_state;
}     

/*****************************************************************************/
//Description : set as a wallpaper
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetWallpaper(void)
{
    MMIDC_SetCurrentPhotoAsWallpaper();
}     
#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//Description : edit pic
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void EditPic(void)
{
    wchar   *full_path_ptr = PNULL;
    uint16  full_path_len = 0;
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    
    photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
    if (PNULL != photo_info_ptr)
    {
        full_path_ptr = photo_info_ptr->file_info.file_name;
        full_path_len = photo_info_ptr->file_info.file_name_len;
    }
    
    MMIAPIPIC_OpenPicEditorMainWin(full_path_ptr, full_path_len);
}
#endif
/*****************************************************************************/
//Description : get pda key by message id
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL  DC_PDA_KEY_E GetPDAKeyByMsgID(MMI_MESSAGE_ID_E msg_id, //
                                             void *param //in
                                             )
{
    MMIDC_PDA_MSG_MAP_T msg_map[]=
    {
        {MSG_APP_WEB, PDA_OK_KEY},
        {MSG_CTL_MIDSK, PDA_OK_KEY},
        {MSG_APP_CAMERA, PDA_OK_KEY},
        {MSG_APP_CANCEL, PDA_BACK_KEY},
        {MSG_CTL_CANCEL, PDA_BACK_KEY},

        {MSG_APP_OK, PDA_LEFT_SOFT_KEY},
        {MSG_CTL_OK, PDA_LEFT_SOFT_KEY},
 
        {MSG_APP_RIGHT, PDA_RIGHT_KEY},
        {MSG_KEYREPEAT_RIGHT, PDA_RIGHT_KEY},
        {MSG_APP_LEFT, PDA_LEFT_KEY},
        {MSG_KEYREPEAT_LEFT, PDA_LEFT_KEY},
        {MSG_APP_UP, PDA_UP_KEY},
        {MSG_KEYREPEAT_UP, PDA_UP_KEY},
        {MSG_APP_DOWN, PDA_DOWN_KEY},
        {MSG_KEYREPEAT_DOWN,PDA_DOWN_KEY},
        {MSG_DC_RETURN_KEY, PDA_RIGHT_SOFT_KEY},
        
        {MSG_APP_MENU, PDA_MENU_KEY},
        
        {MSG_APP_UPSIDE, PDA_UP_KEY},
        {MSG_APP_DOWNSIDE, PDA_DOWN_KEY}
    };

    int16 i = 0;
    int16 total = 0;
    DC_PDA_KEY_E  key = PDA_KEY_MAX;

    total = ARR_SIZE(msg_map);

    for (i = 0; i < total; i++)
    {
        if (msg_id == msg_map[i].msg_id)
        {
            key = msg_map[i].key;
            break;
        }
    }

    return key;
}

/*****************************************************************************/
//Description : handle Web key in PDA Camera;
//Global resource dependence : 
//Author: glen.li
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleWebKeyInPDA(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN ret = FALSE;
    MMI_CTRL_ID_T ctrl_id = 0;

    
    
    ctrl_id = MMK_GetActiveCtrlId(MMIDC_MAIN_WIN_ID);

    if(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID != ctrl_id && (BUTTON_LIST_FOCUS_MODE == s_cur_selection_left_icon || BUTTON_LIST_FOCUS_WHITEBLANCE == s_cur_selection_left_icon 
        ||BUTTON_LIST_FOCUS_EFFECT == s_cur_selection_left_icon || BUTTON_LIST_FOCUS_SETTING == s_cur_selection_left_icon))
    {
        ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
        MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, ctrl_id);
    }
    else if(MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID == ctrl_id && BUTTON_LIST_FOCUS_SETTING == s_cur_selection_left_icon)
    {
        ctrl_id = MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID;
        MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, ctrl_id);
    }


    SCI_TRACE_LOW("[MMIDC] HandleWebKeyInPDA msg_id=0x%X", msg_id);
    SCI_TRACE_LOW("[MMIDC] HandleWebKeyInPDA s_cur_selection_left_icon=0x%X",\
                 s_cur_selection_left_icon);
    

    ret = HandleTPPenOK(ctrl_id);    

    return ret;   
}
/*****************************************************************************/
//Description : handle direction key in PDA Camera;
//Global resource dependence : 
//Author: glen.li
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleDirectionKeyInPDA(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN ret = FALSE;
    MMI_CTRL_ID_T ctrl_id = 0;


    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA msg_id=0x%X",\
                 msg_id);
    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA s_flag_whether_is_right=%d",\
                 s_flag_whether_is_right);
                 
    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA s_cur_selection_left_icon=%d",\
                 s_cur_selection_left_icon);
    
    //get current camera's down and up key at left tab item.
    if(!s_flag_whether_is_right)
    {
        if(MSG_APP_DOWN == msg_id)
        {
            if(BUTTON_LIST_FOCUS_SETTING == s_cur_selection_left_icon)
            {
                s_cur_selection_left_icon = BUTTON_LIST_FOCUS_MODE;
            }
            else
            {
                s_cur_selection_left_icon++;
            }
            
#ifdef MMIDC_F_U_CAMERA
            if (MMIDC_IsUCameraEnable())
            {
                s_cur_selection_left_icon = BUTTON_LIST_FOCUS_SETTING;
            }
#endif             
            ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID + s_cur_selection_left_icon;
        }
        else if(MSG_APP_UP == msg_id)
        {
            if(BUTTON_LIST_FOCUS_MODE == s_cur_selection_left_icon)
            {
                s_cur_selection_left_icon = BUTTON_LIST_FOCUS_SETTING;
            }
            else
            {

                s_cur_selection_left_icon--;
            }

#ifdef MMIDC_F_U_CAMERA
            if (MMIDC_IsUCameraEnable())
            {
                s_cur_selection_left_icon = BUTTON_LIST_FOCUS_SETTING;
            }
#endif 

            ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID + s_cur_selection_left_icon;
        }
    }

    //handle left and right arrow key.
    if(MSG_APP_RIGHT == msg_id)
    {
        if(BUTTON_LIST_FOCUS_MODE == s_cur_selection_left_icon || BUTTON_LIST_FOCUS_WHITEBLANCE == s_cur_selection_left_icon 
            ||BUTTON_LIST_FOCUS_EFFECT == s_cur_selection_left_icon)
        {
            ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, ctrl_id);
            s_flag_whether_is_right = TRUE;
        }

    }
    else if(MSG_APP_LEFT == msg_id)
    {
#ifdef MMIDC_F_U_CAMERA
        if (MMIDC_IsUCameraEnable())
        {
            s_cur_selection_left_icon = BUTTON_LIST_FOCUS_SETTING;
        }
#endif 

        ctrl_id = MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID + s_cur_selection_left_icon;
        s_flag_whether_is_right = FALSE;
    }

    ret = HandleTPPenOK(ctrl_id);

    //NO TP handle when it is BUTTON_LIST_FOCUS_SETTING.
    if(MSG_APP_RIGHT == msg_id)
    {
        if(BUTTON_LIST_FOCUS_SETTING == s_cur_selection_left_icon)
        {
            ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
            MMK_SetAtvCtrl(MMIDC_MAIN_WIN_ID, ctrl_id);
            s_flag_whether_is_right = TRUE;
        }
    }     

    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA2 ctrl_id=0x%X",\
                 ctrl_id);
    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA2 s_flag_whether_is_right=%d",\
                 s_flag_whether_is_right);
                 
    SCI_TRACE_LOW("[MMIDC] HandleDirectionKeyInPDA2 s_cur_selection_left_icon=%d",\
                 s_cur_selection_left_icon);
    
    return ret;   
}
/*****************************************************************************/
//Description : handle pda key
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePDAKey(MMI_MESSAGE_ID_E msg_id, //IN
                           void *param //in
                           )
{
    BOOLEAN ret = FALSE;
    DC_PDA_KEY_E key = PDA_KEY_MAX;

    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();
    BOOLEAN is_btn_expand_press = FALSE;

    is_btn_expand_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    //SCI_TRACE_LOW:"[MMIDC] HandlePDAKey is_btn_expand_press=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8440_112_2_18_2_9_9_310,(uint8*)"d",is_btn_expand_press);
            
    switch(msg_id)
    {
    case MSG_APP_CAMERA:
        if (MMIAPIDC_IsRecording())
        {
            key = PDA_LEFT_SOFT_KEY;
        }
        else
        {
            key = PDA_KEY_MAX;
            if ((DC_PREVIEW_MODE == flow_mode)
                || (DV_PREVIEW_MODE == flow_mode)
               )
            {
                if (!is_btn_expand_press)
                {
                    ProcessCaptureBtn();
                }
            }
        }
        break;
    default:
        key = GetPDAKeyByMsgID(msg_id, param);
        break;
    }
    
    switch(key)
    {
    case PDA_OK_KEY:
        if (is_btn_expand_press)
        {   
            ret = HandleWebKeyInPDA(msg_id);
            break;
        }
#if defined(MMIDC_F_U_CAMERA)
        if (MMIDC_IsUCameraDoing()
#if defined(MMIDC_F_SMILE_AUTO_PHOTO)
            && !MMIDC_IsSmileAutoPhotoEnable()
#endif
            )
        {
            break;
        }
#endif
        
        if ((DC_PREVIEW_MODE == flow_mode)
            || (DV_PREVIEW_MODE == flow_mode)
           )
        {

#ifdef MMIDC_F_N_IN_ONE
            if(MMIDC_IsNInOneEnable()
                && !MMIDC_IsNInOneStartDisplayFrame())
            {
                break;
            }
            else
#endif              
            {
                ProcessCaptureKey();
            }
        }
        if (MMIDC_IsCapturing())
        {
            MMIDC_Saving();
        }
        else
        {
            MMIDC_PDACmd(key);
        }
        break;
    case PDA_BACK_KEY:
    case PDA_RIGHT_SOFT_KEY:      
        MMIDC_PushCaptureCallbackSemaphore();
        MMIDC_PopCaptureCallbackSemaphore();
        if(MMIDC_IsCapturing())
        {
            MMIDC_BackToPreview();
        }
        else
        {
            MMIDC_PDACmd(key);
        }
        break;
    case PDA_LEFT_SOFT_KEY:      
        if(MMIDC_IsCapturing())
        {
            MMIDC_Saving();
        }
        else
        {
            MMIDC_PDACmd(key);
        }
        break;    
    case PDA_RIGHT_KEY:
    case PDA_LEFT_KEY:
    case PDA_DOWN_KEY:
    case PDA_UP_KEY:
        if(is_btn_expand_press)
        {
            ret = HandleDirectionKeyInPDA(msg_id);
        }
        else
        {
            if((PDA_DOWN_KEY == key) || (PDA_UP_KEY == key))
            {
#if defined(MMIDC_F_U_CAMERA)
                if(!MMIDC_GetUCameraModuleFlag())
#endif
                {
                    if(DV_RECORD_MODE != flow_mode 
                        && DV_PAUSE_MODE != flow_mode )
                    {
                        if(!GetIconStateAsPress(PDA_RECT_ZOOM))
                        {
                            ProcessZoomBtn();
                            MMIDC_ShowAllOSD();
                        }
                    }
                    else 
                    {
                        if(!IsDisplaySeekBar())
                        {
                            SetDisplaySeekBarFlag(TRUE);
                            DisplayZoomSeekBar();  
                            MMIDC_CompleteDisplayOSD();
                        }
                    }
                }
            }
        	
            MMIDC_PDACmd(key);
        }
        break;
    default:
        if (key < PDA_KEY_MAX)
        {
            MMIDC_PDACmd(key);
        }
        break;
    }

    //SCI_TRACE_LOW:"[MMIDC] HandlePDAKey msg_id=0x%lX, ret=%d, key=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8542_112_2_18_2_9_9_311,(uint8*)"ddd",msg_id, ret, key);
    
    return ret;
}

/*****************************************************************************/
//Description : restore pda desktop
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void RestorePDADesktop(void)
{
#if !defined(MMIDC_F_DV_ZOOM) || defined(MMIDC_F_U_CAMERA)
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
#endif

#if defined (MMIDC_F_N_IN_ONE)
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();
#endif

   BOOLEAN is_visible = FALSE;
   BOOLEAN is_zoom_press = FALSE;
   
   is_zoom_press = GetIconStateAsPress(PDA_RECT_ZOOM);

   if (!is_zoom_press)
   {
        SetExpandBtnVisible(TRUE);
        ResetExpandBtnRect();
        
        SetCaptureBtnVisible(TRUE);  
        SetSensorBtnVisible(TRUE);  
#ifdef MMIDC_F_N_IN_ONE
        ResetSensorBtn();
#endif

#if defined (MMIDC_F_U_CAMERA)
        if(CAMERA_MODE_DC == mode)
        {
#if defined(MMIDC_F_WORK_MODE)
            if(MMIDC_IsWorkMode())
            {
                SetUCameraBtnVisible(FALSE);
            }
            else
#endif
            {
                SetUCameraBtnVisible(TRUE);
            }
        }
        else
        {
            SetUCameraBtnVisible(FALSE);
        }
        ResetSettingBtn();
#endif        

#if defined(MMIDC_F_DV_ZOOM)
        is_visible = TRUE;
#else
        if (CAMERA_MODE_DC == mode)
        {    
            is_visible = TRUE;  
        }
        else
        {
            is_visible = FALSE;
        }
#endif        
#ifdef MMIDC_F_N_IN_ONE
        if(MMIDC_IsNInOneEnable() 
                    && (DC_PREVIEW_MODE == flow_mode)
                    )
        {
            is_visible = FALSE;
        }
#endif
        SetZoomBtnVisible(is_visible);        
        ResetZoomBtn();

#ifdef MMIDC_F_N_IN_ONE
        if(MMIDC_IsNInOneEnable() 
                && (DC_PREVIEW_MODE == flow_mode)
                && (0 != MMIDC_GetNInOneImageNumber())
                )
        {
            SetPicBtnVisible(FALSE);
        }
        else
        {
            SetPicBtnVisible(TRUE);
            ResetPicBtn();
        }
#else                        
        SetPicBtnVisible(TRUE);
#endif

#if defined(MMIDC_F_WORK_MODE)
            if(MMIDC_IsWorkMode())
            {
                SetPicBtnVisible(FALSE);
            }
#endif

        //DisplayLastThumbnail();
        SetAllCmdBtnsVisible(FALSE);
    }
    
    SetArrowLeftBtnVisible(FALSE);
    SetArrowRightBtnVisible(FALSE);

#if 0//defined(MMIDC_F_EFFECT_360)
    SetEffect360LeftBtnVisible(FALSE);
    SetEffect360RightBtnVisible(FALSE);
#endif

    //SCI_TRACE_LOW:"[MMIDC] RestorePDADesktop is_zoom_press=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8651_112_2_18_2_9_9_312,(uint8*)"d",is_zoom_press);
}
/*****************************************************************************/
//Description : DrawBrightnessBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawBrightnessBmp(void)
{
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = IMG_BRIGHTNESS_ICON;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    GUI_RECT_T          rect = {0};
    MMIDC_PDA_ICON_RECT_E index = 0;

    if(CAMERA_MODE_DC == mode)
    {
        index = PDA_RECT_PIC_PROPERTY_BRIGHTNESS_ICON;
    }
    else
    {
        index = PDA_RECT_PIC_PROPERTY_DV_BRIGHTNESS_ICON;
    }
    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);
    
    x = rect.left;
    y = rect.top;
    
    MMIDC_DisplayBmp(x, y,img_id);
}
/*****************************************************************************/
//Description : DrawContrastBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawContrastBmp(void)
{
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = IMG_CONTRAST_ICON;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E index = 0;
    
    
    if(CAMERA_MODE_DC == mode)
    {
        index = PDA_RECT_PIC_PROPERTY_CONTRAST_ICON;
    }
    else
    {
        index = PDA_RECT_PIC_PROPERTY_DV_CONTRAST_ICON;
    }
    
    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);

    x = rect.left;
    y = rect.top;
    
    MMIDC_DisplayBmp(x, y,img_id);
}
/*****************************************************************************/
//Description : DrawEcBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawEcBmp(void)
{
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = IMG_EC_ICON;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E index = PDA_RECT_PIC_PROPERTY_EC_ICON;
    
    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);

    if(CAMERA_MODE_DC == mode)
    {
        x = rect.left;
        y = rect.top;
        MMIDC_DisplayBmp(x, y,img_id);
    }
}
/*****************************************************************************/
//Description : DrawPicPropertyBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawPicPropertyBmp(void)
{
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = IMG_PIC_PROPERTY_BG;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E index = PDA_RECT_PIC_PROPERTY_SEEKBAR_BG;

    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);

    
    if(CAMERA_MODE_DV == mode)
    {
        img_id = IMG_DV_PIC_PROPERTY_BG;
    }
    x = rect.left;
    y = rect.top;
    
    MMIDC_DisplayBmp(x, y,img_id);
}
/*****************************************************************************/
//Description : DrawPicBoundaryBmp
//Global resource dependence : 
//Author: kunliu
//Note: 
/*****************************************************************************/
LOCAL void DrawPicBoundaryBmp(void)
{    
    BOOLEAN is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    int16 x = 0;
    int16 y = 0;
    MMI_IMAGE_ID_T img_id = IMG_ICONLIST_SHADOW;

    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    GUI_RECT_T rect = {0};
    MMIDC_PDA_ICON_RECT_E  index = PDA_RECT_LIST_SHADOW;

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        img_id = IMG_ICONLIST_SHADOW_V;
    }

    
    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);
        
    x = rect.left;
    y = rect.top;

    if(is_press)
    {
        MMIDC_DisplayBmp(x, y,img_id);
    }
}
/*****************************************************************************/
//Description : DrawPicPropertyBackground
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void DrawPicPropertyBackground(void)
{
    GUI_RECT_T          rect = {0};
#if !defined(MMIDC_F_PDA_BIT_16)    
    GUI_LCD_DEV_INFO    dev_info_ptr = MMIDC_GetOSDBlockInfo();
    MMI_IMAGE_ID_T      img_id = IMG_ICONLIST_TEXT_BG;
#endif
    MMIDC_PDA_ICON_RECT_E  index = PDA_RECT_LIST_RECT_BG;

    rect = GetItemFixedRect(index, TRUE);
    SetIconRect(index, rect);

#if defined(MMIDC_F_PDA_BIT_16)
    MMIDC_ClearRect(rect, MMIDC_PDA_LIST_BG_COLOR);
#else    
    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        MMIDC_MAIN_WIN_ID,
        img_id,
        &dev_info_ptr);
#endif    
}
/*****************************************************************************/
//Description :  clrar list rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearListRect(void)
{
    GUI_RECT_T rect = {0};
    GUI_COLOR_T transparent_color = MMIDC_GetTransparentColor();    
    
    rect = GetItemFixedRect(PDA_RECT_LIST_RECT_BG, TRUE);
    
    MMIDC_ClearRect(rect, transparent_color);
}
/*****************************************************************************/
//Description :  clear arrow btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ClearArrowBtnRect(void)
{
    GUI_RECT_T   left_rect = {0};    
    GUI_RECT_T   right_rect = {0};
    GUI_COLOR_T transparent_color = MMIDC_GetTransparentColor();    


    GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID, &left_rect);
    MMIDC_ClearRect(left_rect, transparent_color);


    GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID, &right_rect);
    MMIDC_ClearRect(right_rect, transparent_color);
}
/*****************************************************************************/
//Description : handle glide expand tp down
//Global resource dependence : 
//Author: kunliu  
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGlideExpandTpDown(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    GUI_RECT_T rect = {0};
    DC_CONTROL_ID_E ctrl_id = MMIDC_PDA_BUTTON_EXPAND_CTRL_ID;
    uint16 screen_width = 0;    
    uint16 screen_height = 0;    
    BOOLEAN is_visible = FALSE;
    BOOLEAN is_press = FALSE;

    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);
    is_visible = IsCtrlVisible(ctrl_id);
    if (is_visible && !is_press)
    {
        GUIAPICTRL_GetRect(ctrl_id, &rect);

        /*Since application has response MSG_CTL_BUTTON_DOWN message,
        so don't response the tp down message when click on button*/
        if(!GUI_PointIsInRect(point, rect)) 
        {
            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                rect.left = 0;
                rect.right = screen_width - 1;
            }
            else
            {
                rect.top = 0;
                rect.bottom = screen_height - 1;
            }
            if(GUI_PointIsInRect(point, rect))
            {
                s_pda_expand_info.down_point = point;
                s_pda_expand_info.is_glide = TRUE;
                ret = TRUE;
            }
        }
    }
    SCI_TRACE_LOW("[MMIDC] HandleGlideExpandTpDown - visible(%d),glide(%d),press(%d),point(%d,%d) ", 
        is_visible, is_press, s_pda_expand_info.is_glide,s_pda_expand_info.down_point.x, s_pda_expand_info.down_point.y);

    return ret;
}
/*****************************************************************************/
//Description : handle glide expand tp up
//Global resource dependence : 
//Author: kunliu  
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGlideExpandTpUp(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;    
    BOOLEAN is_press = FALSE;
    DC_CONTROL_ID_E ctrl_id = MMIDC_PDA_BUTTON_EXPAND_CTRL_ID;
    int16 glide_len = 0;    
    BOOLEAN is_visible = FALSE;
    BOOLEAN is_glide = FALSE;
    GUI_RECT_T rect = {0};
    
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    is_visible = IsCtrlVisible(ctrl_id);
    is_glide = s_pda_expand_info.is_glide;
    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);

    if (is_visible && is_glide && !is_press)
    {
        /*If down point, up point are both on button, application will 
        response MSG_CTL_PENOK message, so don't response here*/
        GUIAPICTRL_GetRect(ctrl_id, &rect);
        
        if(!GUI_PointIsInRect(point, rect) || 
            !GUI_PointIsInRect(s_pda_expand_info.down_point, rect))
        {
            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                glide_len = point.y - s_pda_expand_info.down_point.y;
            }
            else
            {
                glide_len = point.x - s_pda_expand_info.down_point.x;
            }
            
            if(glide_len >= 0)        
            {
                ProcessExpandBtn(MMIDC_PDA_BUTTON_EXPAND_CTRL_ID);
                ret = TRUE;
            }
        }
    }


    SCI_TRACE_LOW("[MMIDC] HandleGlideExpandTpUp is_visible =%d, is_glide=%d, is_press=%d", is_visible, is_glide, is_press);
            
    //SCI_TRACE_LOW:"[MMIDC] HandleGlideExpandTpUp ret =%d, is_move_panel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_8963_112_2_18_2_9_10_314,(uint8*)"dd",ret, glide_len);
    
    return ret;
}

/*****************************************************************************/
//Description : Hide panel items
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HidePanelItems(MMI_HANDLE_T ctrl_id)
{
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    
    GUI_RECT_T rect = {0};    
    MMIDC_LIST_CTRL_ITEM_T list_item = {0};
    MMI_CTRL_ID_T list_ctrl_id = 0;
    BOOLEAN is_show = FALSE;
    int16 move_len = 0;
    MMIDC_ITEM_CONTENT_T content = {0};

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        move_len = -MOVE_HEIGHT_V;
    }
    else
    {
        move_len = -MOVE_WIDTH;
    }

    GetListCtrlItem(&list_item);
    switch(list_item.icon_id)
    {
        case PDA_ICON_SETTING_DC_FLASH:
        case PDA_ICON_SETTING_DC_QUALITY:
        case PDA_ICON_SETTING_DC_SIZE:
        case PDA_ICON_SETTING_DC_PIC_MODE:
        case PDA_ICON_SETTING_DC_SELF_SHOT:   
        case PDA_ICON_SETTING_DC_CORUSCATE:   
        case PDA_ICON_SETTING_DC_STORAGE:
        case PDA_ICON_SETTING_DV_FLASH:
        case PDA_ICON_SETTING_DV_SIZE:   
        case PDA_ICON_SETTING_DV_CORUSCATE: 
        case PDA_ICON_SETTING_DV_VIDEOFORMAT:
        case PDA_ICON_SETTING_DV_STORAGE:
            list_ctrl_id = MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID;
        break;
        default:
            list_ctrl_id = MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID;
        break;
    }
    
    //mode button list
    MoveButtonList(move_len, is_show);

    //list canvas
    MoveTypeListCanvas(move_len, list_ctrl_id, is_show);
    
    //list shadow
    content.type = MMIDC_ITEM_TYPE_SELF_DRAW;
    content.item_data.index = PDA_RECT_LIST_SHADOW;
    rect = GetMovedRectAndChangeRect(content, move_len);

    //expand ctrl
    content.type = MMIDC_ITEM_TYPE_CTRL;
    content.item_data.ctrl_id = ctrl_id;
    rect = GetMovedRectAndChangeRect(content, move_len);

    return TRUE;
}

/*****************************************************************************/
//Description : hide expand panel button
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HideExpandPanelBtn(MMI_HANDLE_T ctrl_id
                                )
{
    BOOLEAN ret   = FALSE;
    BOOLEAN is_press = FALSE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();


    is_press = GetIconStateAsPress(PDA_RECT_EXPAND);       
    if(is_press)
    {            
        SetIconStateAsPress(PDA_RECT_EXPAND, FALSE);
        SetIconStateAsPress(PDA_RECT_IMAGE_PROPERTY, FALSE);

        HidePanelItems(ctrl_id);
        
        ResetExpandBtn();
        ResetCaptureBtn(mode);
        ResetSensorBtn();
        ResetPicBtn();
#if defined(MMIDC_F_U_CAMERA)
        ResetUCameraBtn();
#endif
    }

    return ret;
}
/*****************************************************************************/
//Description :  clrar expand rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearExpandPanelRect(void)
{

    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    GUI_RECT_T rect = {0};
    uint16 width = 0;
    uint16 height = 0;
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &width, &height);

#if defined(MAINLCD_SIZE_240X320)
    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        rect.bottom = MOVE_HEIGHT_V - 1; ;
        rect.right = width - 1;
    }
    else
    {
        rect.bottom = height - 1;
        rect.right = MOVE_WIDTH - 1;    
    }
    
    MMIDC_ClearRect(rect, color);

    //expand button rect
    rect = GetItemFixedRect(PDA_RECT_EXPAND, TRUE);
    MMIDC_ClearRect(rect, color);
#else
    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        rect.bottom = MOVE_HEIGHT_V + EXPAND_BTN_HEIGHT_V - 1; ;
        rect.right = width - 1;
    }
    else
    {
        rect.bottom = height - 1;
        rect.right = MOVE_WIDTH + EXPAND_BTN_WIDTH - 1;    
    }
    
    MMIDC_ClearRect(rect, color);

    /*
    * mark under code,because hor screen brush slowly
    *for cr NEWMS00131395
    */
    //expand button rect
    //rect = GetItemFixedRect(PDA_RECT_EXPAND, TRUE);
    //MMIDC_ClearRect(rect, color);
#endif
}

/*****************************************************************************/
//Description : set desktop visible or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
//PUBLIC void MMIDC_SetDesktopVisibility(BOOLEAN is_visible)
//{
//#ifdef MMIDC_DESKTOP_ICON_BAR
//    //SetDesktopVisible(is_visible);
//#endif
//}

/*****************************************************************************/
//Description : create desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDC_CreateDesktopPDA(void)
{
    return CreateDesktopPDA();
}

/*****************************************************************************/
//Description : destroy desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyDesktopPDA(void)
{
    DestroyDesktopPDA();
}

/*****************************************************************************/
//Description : process msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DesktopProcessMsg(MMI_MESSAGE_ID_E msg_id, //in
                                       void *param //in
                                       )
{
    return DesktopPDAProcessMsg(msg_id, param);
}

/*****************************************************************************/
//Description :  open pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
//PUBLIC void MMIDC_OpenPDADesktop(void)
//{
//}

/*****************************************************************************/
//Description :  display pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayPDADesktop(void)
{
    return DisplayPDAAllItems();
}

/*****************************************************************************/
//Description :  display button cmd image
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayAllCmdBtns(DC_FLOW_MODE_E mode)
{
    DisplayAllCmdBtns(mode);
}

/*****************************************************************************/
//Description :  restore to pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_RestorePDADesktop(void)
{
    RestorePDADesktop();    
}

/*****************************************************************************/
//Description :  close all pda timer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAllPDATimer(void)
{
    StopZoomSeekBarTimer();
    StopImageAttrSeekBarTimer();
    StopZoomSeekBarDisplayTimer();
}

/*****************************************************************************/
//Description :  reset all pda ctrls dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetAllPDACtrlsDevInfo(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;    
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    uint32 i = 0;
    
    for (i = MMIDC_PDA_CTRL_ID_START; i < MMIDC_PDA_CTRL_ID_END; i++)
    {
        if (PNULL != MMK_GetCtrlHandleByWin(win_id, i))
        {
           MMK_SetCtrlLcdDevInfo(i, &dev_info);
        }
    }
}

/*****************************************************************************/
//Description :  start preview anim
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartPreviewAnim(MMIDC_PREVIEW_ANIM_STEP_E step 
                                   )
{
#if defined(MMIDC_F_PREVIEW_ANIM)
    MMI_WIN_ID_T wait_win_id = MMIDC_MAIN_WAIT_WIN_ID;
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    SCREEN_MODE_E screen_mode = 0;
    
    MMI_IMAGE_ID_T img_id = IMG_PREVIEW_ANIM_1;
    GUI_POINT_T dis_point = {0};
    uint32 i = 0;
    uint32 j = 0;
    uint32 total_num = 4;
    uint32 total_block = 2;

    uint32 screen_height = 0;
    uint32 screen_width = 0;
    LCD_ANGLE_E angle = 0;
    
    //SCI_TRACE_LOW:"[MMIDC MMIDC_StartPreviewAnim step=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_9257_112_2_18_2_9_11_315,(uint8*)"d",step);
        

    screen_mode = MMIDC_GetScreenMode();
    
    switch(step)
    {
    case PREVIEW_ANIM_STEP_IMG:
        MMIDC_SetFirstPreviewFlag(TRUE);

        win_id = MMK_GetFocusWinId();   

        if (wait_win_id == win_id)
        {
            if(SCREEN_MODE_VERTICAL == screen_mode)
            {
                angle = LCD_ANGLE_0;
            }
            else
            {
                angle = LCD_ANGLE_90;
            }
            MMK_SetWinAngle(win_id, angle, FALSE);
            MMITHEME_SetUpdateDelayCount(0);
        }

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            img_id = IMG_PREVIEW_ANIM_1_V;
        }
        else
        {
            img_id = IMG_PREVIEW_ANIM_1;
        }


        GUIRES_DisplayImg(&dis_point,
                        PNULL,
                        PNULL,
                        win_id,
                        img_id,
                        &dev_info);
            
        MMIDC_CompleteDisplayOSD();       
        
        break;
    case PREVIEW_ANIM_STEP_ANIM:  
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_StartPreviewAnim start anim time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_9305_112_2_18_2_9_11_316,(uint8*)"d",SCI_GetTickCount());

        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            img_id = IMG_PREVIEW_ANIM_2_1_V;
        }
        else
        {
            img_id = IMG_PREVIEW_ANIM_2_1;
        }

        screen_width = MMIDC_GetPreviewWidth();
        screen_height = MMIDC_GetPreviewHeight();                        
        for(i = 0; i < total_num; i++)
        {
            MMIDC_ClearOSDBlock();
            DisplayPDAAllItems();

            for (j = 0; j < total_block; j++)
            {
                if (SCREEN_MODE_VERTICAL == screen_mode)
                {
                    switch(j)
                    {
                    case 0:
                        dis_point.x = 0;
                        dis_point.y = 0;
                        break;
                    case 1:
                        dis_point.x = screen_width / total_block;
                        dis_point.y = 0;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch(j)
                    {
                    case 0:
                        dis_point.x = 0;
                        dis_point.y = 0;
                        break;
                    case 1:
                        dis_point.x = 0;
                        dis_point.y = screen_height / total_block;
                        break;
                    default:
                        break;
                    }
                }

                
                GUIRES_DisplayImg(&dis_point,
                                PNULL,
                                PNULL,
                                win_id,
                                img_id + i * total_block + j,
                                &dev_info
                                );
                                
            }                            
            MMIDC_CompleteDisplayOSD();

            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                SCI_Sleep(MMIDC_ANIM_VERTICAL_SLEEP_TIME);
            }
        }

        MMIDC_ClearOSDBlock();
        DisplayPDAAllItems();
        MMIDC_CompleteDisplayOSD();

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_StartPreviewAnim end_time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_9381_112_2_18_2_9_11_317,(uint8*)"d",SCI_GetTickCount());

        break;
    default:
        break;
    }
#endif
}

#ifdef MMIDC_F_CAPTURE_ANIM
/*****************************************************************************/
//Description :  start capture anim before
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartCaptureAnimBefore(void)
{
}

/*****************************************************************************/
//Description :  start capture anim after
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void StartCaptureAnimAfter(void)
{
}
#endif

/*****************************************************************************/
//Description :  start capture anim after
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_PDAForLoseFocus(void)
{
#if defined(MMIDC_F_DV_ZOOM)    
    MMIDC_ZOOM_SEEKBAR_INFO_T seekbar_info = GetZoomSeekBarInfo();
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();
#endif
    MMI_CTRL_ID_T ctrl_array[] = 
    {
        MMIDC_PDA_BUTTON_CMD_1_CTRL_ID,
        MMIDC_PDA_BUTTON_CMD_2_CTRL_ID,
        MMIDC_PDA_BUTTON_CMD_3_CTRL_ID,
        MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID,
        MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID
    };
    uint32 i = 0;
    
    for (i = 0; i < ARR_SIZE(ctrl_array); i++)
    {
        SetCtrlVisible(ctrl_array[i], FALSE);
    }

#if defined(MMIDC_F_DV_ZOOM)    
    if( DV_RECORD_MODE == flow_mode
		|| DV_PAUSE_MODE == flow_mode)
    {
        seekbar_info.is_display_seekbar = FALSE;
        seekbar_info.is_tp_down = FALSE;
        SetZoomSeekBarInfo(&seekbar_info);
    }
#endif
}
#if 0
/*****************************************************************************/
//Description :  reset ctrl rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetPDACtrlRect(void)
{

    GUI_BOTH_RECT_T both_rect = {0};
    MMI_CTRL_ID_T ctrl_array[] = 
    {
        MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID,
        MMIDC_PDA_BUTTON_SENSOR_CTRL_ID,
        MMIDC_PDA_BUTTON_ZOOM_CTRL_ID,
        MMIDC_PDA_BUTTON_EXPAND_CTRL_ID,
        MMIDC_PDA_BUTTON_PIC_CTRL_ID
    };
    int32 i = 0;
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    
    for (i = 0; i < ARR_SIZE(ctrl_array); i++)
    {
        GUIAPICTRL_GetBothRect(ctrl_array[i], &both_rect);
        
        if (SCREEN_MODE_VERTICAL == screen_mode)
        {
            GUIAPICTRL_SetRect(ctrl_array[i],&both_rect.v_rect);
        }
        else
        {
            GUIAPICTRL_SetRect(ctrl_array[i],&both_rect.h_rect);
        }
    }
}
#endif

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description :  get zoom seekbar width
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetZoomSeekBarWidth(void)
{
    uint16 width = 0;

    if (PNULL != s_desktop_pda_ptr)
    {
        width = s_desktop_pda_ptr->zoom_seekbar_width;
    }
    
    return width;
}

/*****************************************************************************/
//Description :  set zoom seekbar width
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetZoomSeekBarWidth(uint16 width)
{
    if (PNULL != s_desktop_pda_ptr)
    {
        s_desktop_pda_ptr->zoom_seekbar_width = width;
    }
}

#endif //#if defined(MMIDC_F_DV_ZOOM)


/*****************************************************************************/
//Description :  update zoom seekbar
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_UpdateZoomSeekBar(void)
{
    BOOLEAN is_zoom_btn_press = FALSE;
    MMIDC_ZOOM_SEEKBAR_INFO_T zoom_info = GetZoomSeekBarInfo();
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();

#if defined(MMIDC_F_DV_ZOOM)
    uint32 max_zoom = 0;
    DC_FLOW_MODE_E flow_mode = MMIDC_GetCurrentMode();

    if(CAMERA_MODE_DC == mode)
    {
        max_zoom = MMIDC_GetMaxZoomLevel();
    }
    else
    {
        max_zoom = MMIDC_GetDVMaxZoomLevel();
    }
    

    if ((DV_RECORD_MODE == flow_mode)
        || (DV_PAUSE_MODE == flow_mode)
        )
    {
        if (max_zoom > 1)
        {
            is_zoom_btn_press = TRUE;
        }
    }
    else
#endif  
    {
        is_zoom_btn_press = GetIconStateAsPress(PDA_RECT_ZOOM);
    }

    if (is_zoom_btn_press)
    {
        if(CAMERA_MODE_DC == mode)
        {
            zoom_info.seekbar_info.cur_value = MMIDC_GetPhotoZoomValue();
        }
#if defined(MMIDC_F_DV_ZOOM)
        else
        {
            zoom_info.seekbar_info.cur_value = MMIDC_GetDVZoomValue();
        }
#endif
        if(0 != zoom_info.seekbar_info.total_value)
        {
            zoom_info.seekbar_info.cur_step = zoom_info.seekbar_info.cur_value * 
                                        zoom_info.seekbar_info.total_step / zoom_info.seekbar_info.total_value;
        }
    
        SetZoomSeekBarInfo(&zoom_info);
        
        DrawSeekBarScale(&(zoom_info.seekbar_info)); 
        
        MMIDC_CompleteDisplayOSD();      
    }


}



#if 0//defined(MMIDC_F_EFFECT_360)
/*****************************************************************************/
//Description :  get effect 360 left button both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnEffect360LeftBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                            )
{
    GUI_RECT_T rect = {0};

    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    uint16 img_width = 0;
    uint16 img_height = 0;
#if !defined(MMIDC_F_PDA_BIT_16)
    MMI_IMAGE_ID_T bg_id = IMG_BTN_BG;
#endif
    
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    int16 left = 0;
    int16 top = 0;
    int16  max_key_width = MMIDC_GetMaxWidthOfSoftkey();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();    
    
#if !defined(MMIDC_F_PDA_BIT_16)
    GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_id, win_id); 
#endif

    left = MMIDC_PDA_EFFECT_360_ARROW_SPACE;
    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        left += max_key_width;
    }
    top = (screen_height - img_height) / 2;

    
    rect.left = left;
    rect.top = top;
    both_rect_ptr->h_rect = rect;
    
    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, IMG_BTN_ARROW_LEFT_FOCUS, IMG_BTN_ARROW_LEFT_FOCUS);
}

/*****************************************************************************/
//Description :  set effect 360 left btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetEffect360LeftBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset effect 360 left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetEffect360LeftBtn(BOOLEAN is_gray
                             )
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    GUI_BG_T fg_info = {0}; 

    fg_info.bg_type = GUI_BG_IMG;    
    fg_info.img_id = IMG_BTN_ARROW_LEFT_FOCUS;
    

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_ARROW_LEFT;

    if(is_gray)
    {
        bg_info.img_id = IMG_BTN_ARROW_LEFT_GRAY;
    }
    GUIBUTTON_SetGrayed(ctrl_id, is_gray, &bg_info, FALSE);
    
    GUIBUTTON_SetPressedFg(ctrl_id,&fg_info);    
    GUIBUTTON_SetBg(ctrl_id,&bg_info); 
}

/*****************************************************************************/
//Description :  create effect 360 left button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateEffect360LeftBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnEffect360LeftBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID, &both_rect, IMG_BTN_ARROW_LEFT_FOCUS);
    
    ResetEffect360LeftBtn(FALSE);
}
/*****************************************************************************/
//Description :  get effect 360 right button both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetBtnEffect360RightBothRect(GUI_BOTH_RECT_T *both_rect_ptr //in
                                
                                )
{
    GUI_RECT_T rect = {0};
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    uint16 img_width = 0;
    uint16 img_height = 0;    
    MMI_IMAGE_ID_T img_id = IMG_BTN_ARROW_RIGHT_FOCUS;
    
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    int16 right = 0;
    int16 top = 0;

    int16  max_key_width = MMIDC_GetMaxWidthOfSoftkey();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();    

    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id); 

    right = screen_width - MMIDC_PDA_EFFECT_360_ARROW_SPACE;
    if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
        right -= max_key_width;
    }
    top = (screen_height - img_height) / 2;


    rect.right = right;
    rect.left = rect.right - img_width + 1 ;
    rect.top = top;
    both_rect_ptr->h_rect = rect;

    both_rect_ptr->v_rect = rect;
    
    GetBtnBothRect(both_rect_ptr, img_id, img_id);
}

/*****************************************************************************/
//Description :  set effect 360 right btn visible
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SetEffect360RightBtnVisible(BOOLEAN is_visible)
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID;
    
    SetCtrlVisible(ctrl_id, is_visible);
}

/*****************************************************************************/
//Description :  reset effect 360 right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetEffect360RightBtn(BOOLEAN is_gray
                              )
{
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID;
    GUI_BG_T bg_info = {0}; 
    GUI_BG_T fg_info = {0}; 
    
    fg_info.bg_type = GUI_BG_IMG;    
    fg_info.img_id = IMG_BTN_ARROW_RIGHT_FOCUS;

    bg_info.bg_type = GUI_BG_IMG;    
    bg_info.img_id = IMG_BTN_ARROW_RIGHT;

    if(is_gray)
    {
        bg_info.img_id = IMG_BTN_ARROW_RIGHT_GRAY;
    }
    GUIBUTTON_SetGrayed(ctrl_id, is_gray, &bg_info, FALSE);
    
    GUIBUTTON_SetPressedFg(ctrl_id,&fg_info);    
    GUIBUTTON_SetBg(ctrl_id,&bg_info);
}

/*****************************************************************************/
//Description :  create effect 360 right button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateEffect360RightBtn(void)
{
    GUI_BOTH_RECT_T   both_rect = {0};
    
    GetBtnEffect360RightBothRect(&both_rect);
    CreateCameraBtn(MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID, &both_rect, IMG_BTN_ARROW_RIGHT_FOCUS);
    
    ResetEffect360RightBtn(FALSE);
}

/*****************************************************************************/
//Description :  clear effect 360 arrow btn
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
LOCAL void ClearEffect360ArrowBtnRect(void)
{
    GUI_RECT_T   left_rect = {0};    
    GUI_RECT_T   right_rect = {0};
    GUI_COLOR_T transparent_color = MMIDC_GetTransparentColor();    


    GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID, &left_rect);
    MMIDC_ClearRect(left_rect, transparent_color);


    GUIAPICTRL_GetRect(MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID, &right_rect);
    MMIDC_ClearRect(right_rect, transparent_color);
}

/*****************************************************************************/
//Description : display effect 360 arrow buttons
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayEffect360ArrowBtns(void)
{
#if 0
    MMI_CTRL_ID_T left_ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID;
    MMI_CTRL_ID_T right_ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID;
    
    DC_FLOW_MODE_E flow_mode = 0;
    uint32 total_photo_number = 0;
    uint32 cur_photo_id = 0;
    BOOLEAN is_visible = FALSE;
    BOOLEAN is_left_gray = FALSE;
    BOOLEAN is_right_gray = FALSE;

    GUI_BOTH_RECT_T   both_rect = {0};

    flow_mode = MMIDC_GetCurrentMode();
    
    if (DC_REVIEW_MODE != flow_mode
       )
    {
        return;
    }
    
    
    GetBtnEffect360LeftBothRect(&both_rect);
    GUIAPICTRL_SetBothRect(MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID, &both_rect);
    GetBtnEffect360RightBothRect(&both_rect);
    GUIAPICTRL_SetBothRect(MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID, &both_rect);



    cur_photo_id = MMIDC_GetCurrentPhotoID();
    total_photo_number = EFFECT_360_TYPE_MAX;
    
    if (1 < total_photo_number)    
    {
        is_visible = TRUE;
        SetEffect360LeftBtnVisible(is_visible);
        SetEffect360RightBtnVisible(is_visible);
        
        if (0 == cur_photo_id)
        {
            is_left_gray = TRUE;
        }
        if (cur_photo_id == (total_photo_number - 1) )
        {
            is_right_gray = TRUE;
        }
        
        ClearEffect360ArrowBtnRect();
        ResetEffect360LeftBtn(is_left_gray);
        ResetEffect360RightBtn(is_right_gray);
        
        MMK_SendMsg(left_ctrl_id, MSG_CTL_PAINT, PNULL);
        MMK_SendMsg(right_ctrl_id, MSG_CTL_PAINT, PNULL);
    }
#endif
}

/*****************************************************************************/
//Description : process effect 360 left btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessEffect360LeftBtn(void)
{
    BOOLEAN ret = FALSE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID;

    BOOLEAN is_visible = FALSE;

    is_visible = IsCtrlVisible(ctrl_id);
    if(is_visible)
    {
        //特效切换
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessEffect360LeftBtn ret=%d, is_visible=%D"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_9859_112_2_18_2_9_12_318,(uint8*)"dd",ret, is_visible);
             
    return ret;
}

/*****************************************************************************/
//Description : process effect 360 right btn
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessEffect360RightBtn(void)
{
    BOOLEAN ret = FALSE;
    MMI_HANDLE_T win_id = MMIDC_MAIN_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID;

    BOOLEAN is_visible = FALSE;

    is_visible = IsCtrlVisible(ctrl_id);
    if(is_visible)
    {
        //特效切换
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] ProcessEffect360RightBtn ret=%d, is_visible=%D"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_9884_112_2_18_2_9_12_319,(uint8*)"dd",ret, is_visible);
             
    return ret;
}
#endif //#if defined(MMIDC_F_EFFECT_360)

/*****************************************************************************/
//Description : init pda key status parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitPDAKeyStatusParameter(void)
{
    s_flag_whether_is_right = FALSE;
    s_cur_selection_left_icon = 0;
}

#ifdef MMIDC_F_LAST_IMAGE
/*****************************************************************************/
//Description : get last thumbnail info ptr
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC MMIDC_IMG_INFO_T* MMIDC_GetLastThumbnailInfoPtr(void)
{

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetLastThumbnailInfoPtr");
    
    return s_last_thumbnail_info_ptr;
}

/*****************************************************************************/
//Description :  4 align
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC uint32 GetAlign4Offset(uint32 x)
{
    uint32 z = 0;
    uint32 offset = 0;

    z = x % 4;
    if (z == 0) //do not need align
    {
        offset = 0;
    }
    else
    {
        offset = 4 - z;  //test: if x==7, offset = 4 - 3 = 1
    }

    return offset;
}

/*****************************************************************************/
//Description :  init last thumbnail info
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateLastThumbnailInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    uint32 img_data_size = 0;
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
    uint32 data_unit = 4;
    uint16 img_width = 0;
    uint16 img_height = 0;

#if defined(MMIDC_F_PDA_BIT_16)
    data_unit = 2;
#endif    
    do
    {
        if(PNULL == img_info_ptr)
        {
            img_info_ptr = SCI_ALLOCA(sizeof(MMIDC_IMG_INFO_T));
            if(PNULL == img_info_ptr)
            {
                break;
            }
            s_last_thumbnail_info_ptr = img_info_ptr;
        }
        SCI_MEMSET(img_info_ptr, 0x00, sizeof(*img_info_ptr));
        img_info_ptr->is_img_data_valid = FALSE;
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_BTN_PREVIEW_IMG, win_id);
        img_info_ptr->img_width = img_width;
        img_info_ptr->img_height = img_height;        
        img_info_ptr->img_width += GetAlign4Offset(img_width);
        img_info_ptr->img_height += GetAlign4Offset(img_height);
        img_info_ptr->img_width -= 8;
        img_info_ptr->img_height -= 8;
        img_info_ptr->offset_x = (img_width - img_info_ptr->img_width)/2;
        img_info_ptr->offset_y = (img_height - img_info_ptr->img_height)/2;
        
        img_data_size = img_info_ptr->img_width * img_info_ptr->img_height * data_unit;
        img_info_ptr->img_data_buffer = SCI_ALLOCA(img_data_size);
        if(PNULL == img_info_ptr->img_data_buffer)
        {
            break;
        }
        img_info_ptr->img_data_size = img_data_size;
        
        ret = TRUE;
        SCI_TRACE_LOW("[MMIDC] MMIDC_CreateLastThumbnailInfo img_data_size = %d", img_info_ptr->img_data_size);
    }while(0);
    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateLastThumbnailInfo ret = %d", ret);
    
    return ret;
}

/*****************************************************************************/
//Description :  get last thumbnail data file name
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL uint32 GetLastThumbnailDataFileName(CAMERA_MODE_E mode, wchar* full_path_ptr, uint32 path_len)
{
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    uint32 save_path_len = 0;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SYSTEM;
    wchar* default_path = PNULL;
    const wchar thumbnail_name[]=
    {
        L"_@#dctempthumbnail#@"
    };

    if(PNULL == full_path_ptr)
    {
        return 0;
    }
    if(CAMERA_MODE_DC == mode)
    {
        file_dev = MMIDC_GetPhotoStorageDevice();
        default_path = MMIDC_GetPhotoDefaultPathName();
    }
    else
    {
        file_dev = MMIDC_GetVideoStorageDevice();
        default_path = MMIMULTIM_DIR_MOVIE;
    }
    //MMIDC_CombinePath(save_path,MMIDC_FULL_PATH_LENGTH,file_dev,default_path);
    MMIAPIFMM_CombineFullPathEx(save_path, MMIDC_FULL_PATH_LENGTH,file_dev,default_path, thumbnail_name);

    save_path_len = MMIAPICOM_Wstrlen(save_path);
    MMI_WSTRNCPY(full_path_ptr, 
            path_len, 
            save_path, 
            save_path_len, 
            save_path_len);    

    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDataFileName save_path_len = %d", save_path_len);
    
    return save_path_len;
    
}

/*****************************************************************************/
//Description :  save last thumbnail data to file
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveLastThumbnailDataToFile(void)
{
    MMIDC_SAVE_THUMBNAIL_DATA_T save_date = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    SFS_HANDLE      sfs_handle = 0;
    uint32 writen_size = 0;
    SFS_ERROR_E ret = SFS_ERROR_NONE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();

    if(PNULL == img_info_ptr
        || PNULL == img_info_ptr->img_data_buffer)
    {
        return;
    }

    GetLastThumbnailDataFileName(mode, save_path, MMIDC_FULL_PATH_LENGTH);
    if(img_info_ptr->is_img_data_valid)
    {            
        MMI_WSTRNCPY(save_date.full_path_name, 
                MMIDC_FULL_PATH_LENGTH, 
                img_info_ptr->full_path_name, 
                MMIAPICOM_Wstrlen((const wchar *) img_info_ptr->full_path_name), 
                MMIAPICOM_Wstrlen((const wchar *) img_info_ptr->full_path_name));
        
        save_date.actual_width = img_info_ptr->actual_width;
        save_date.actual_height = img_info_ptr->actual_height;
        save_date.img_data_size = img_info_ptr->img_data_size;

        sfs_handle = SFS_CreateFile(save_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/

        if(SFS_INVALID_HANDLE != sfs_handle)
        {
            ret = SFS_WriteFile(sfs_handle, (const void *)&save_date, sizeof(save_date), &writen_size, NULL);
            SCI_TRACE_LOW("[MMIDC] MMIDC_SaveLastThumbnailDataToFile ret1 = %d", ret);
            if(SFS_ERROR_NONE == ret)
            {
                ret = SFS_WriteFile(sfs_handle, (const void *)img_info_ptr->img_data_buffer, img_info_ptr->img_data_size, &writen_size, NULL);
                SCI_TRACE_LOW("[MMIDC] MMIDC_SaveLastThumbnailDataToFile ret2 = %d", ret);
            }
        }

        SFS_CloseFile(sfs_handle);

        MMIAPIFMM_SetAttr(save_path, MMIAPICOM_Wstrlen((const wchar *) save_path), \
								TRUE, FALSE, FALSE, FALSE);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_SaveLastThumbnailDataToFile ");
}

/*****************************************************************************/
//Description :  save last thumbnail data to file
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadLastThumbnailDataFromFile(CAMERA_MODE_E mode)
{
    MMIDC_SAVE_THUMBNAIL_DATA_T save_date = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    wchar save_path[MMIDC_FULL_PATH_LENGTH + 1] = {0};
    SFS_HANDLE      sfs_handle = 0;
    uint32 readed_size = 0;
    uint32 read_size = 0;
    SFS_ERROR_E ret = SFS_ERROR_NONE;
    BOOLEAN result = FALSE;

    if(PNULL == img_info_ptr
        || PNULL == img_info_ptr->img_data_buffer)
    {
        return result;
    }

    GetLastThumbnailDataFileName(mode, save_path, MMIDC_FULL_PATH_LENGTH);
    
    sfs_handle = SFS_CreateFile(save_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/

    do
    {
        if(SFS_INVALID_HANDLE != sfs_handle)
        {
            read_size = sizeof(save_date);
            ret = SFS_ReadFile(sfs_handle, (void *)&save_date, read_size, &readed_size, NULL);
            SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile 1 ret = %d, read_size = %d, readed_size = %d", 
                ret, read_size, readed_size);
            SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile img_info_ptr->img_data_size = %d, save_date.img_data_size = %d", 
                img_info_ptr->img_data_size, save_date.img_data_size);
            if(SFS_ERROR_NONE != ret
                || read_size != readed_size
                || img_info_ptr->img_data_size != save_date.img_data_size)
            {
                break;
            }
            
            if(!MMIAPIFMM_IsFileExist(save_date.full_path_name, MMIDC_FULL_PATH_LENGTH))
            {
                SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile src file not exist");
                break;
            }
            
            read_size = save_date.img_data_size;
            ret = SFS_ReadFile(sfs_handle, (void *)img_info_ptr->img_data_buffer, read_size, &readed_size, NULL);
            SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile 2 ret = %d, read_size = %d, readed_size = %d", 
                ret, read_size, readed_size);
            if(SFS_ERROR_NONE != ret
                || read_size != readed_size)
            {
                SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile  error");
                break;
            }

            SCI_MEMSET(img_info_ptr->full_path_name, 0, sizeof(img_info_ptr->full_path_name));
            MMI_WSTRNCPY(img_info_ptr->full_path_name,
                MMIDC_FULL_PATH_LENGTH, 
                save_date.full_path_name, 
                MMIAPICOM_Wstrlen((const wchar *) save_date.full_path_name), 
                MMIAPICOM_Wstrlen((const wchar *) save_date.full_path_name));
            
            img_info_ptr->actual_width = save_date.actual_width;
            img_info_ptr->actual_height = save_date.actual_height;
            MMIDC_SetLastThumbnailDataValidFlag(TRUE);
            
            result = TRUE;
        }
    }while(0);

    SFS_CloseFile(sfs_handle);

    SCI_TRACE_LOW("[MMIDC] ReadLastThumbnailDataFromFile result = %d", result);
    
    return result;
}

/*****************************************************************************/
//Description :  destroy last thumbnail info data
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyLastThumbnailInfoData(void)
{
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    
    if (PNULL != img_info_ptr)
    {
        MMIDC_SaveLastThumbnailDataToFile();

        if(PNULL != img_info_ptr->img_data_buffer)
        {
            SCI_FREE(img_info_ptr->img_data_buffer);
            img_info_ptr->img_data_buffer = PNULL;
        }
        img_info_ptr->is_img_data_valid = FALSE;

        SCI_FREE(img_info_ptr);
        img_info_ptr = PNULL;
        s_last_thumbnail_info_ptr = PNULL;
    }
    SCI_TRACE_LOW("[MMIDC] MMIDC_DestroyLastThumbnailInfoData ");
}

#ifdef  VIDEOTHUMBNAIL_SUPPORT
/*****************************************************************************/
//  Description : get video thumbnail callback
//  Global resource dependence : 
//  Author: chunyou
//  Note:  
/*****************************************************************************/

LOCAL  BOOLEAN  GetVideoThumbnailCallback(DPLAYER_IMAGE_DATA_T *image_data_ptr)
{
    BOOLEAN result = FALSE;    
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    wchar* video_fullpathname=PNULL;
    MMIDC_CMD_PARAM_T cmd_param = {0};
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
    uint32 t1 = 0;

    if(PNULL == img_info_ptr
        || PNULL == image_data_ptr
        || PNULL == img_info_ptr->img_data_buffer)
    {
        return result;
    }
    t1 = SCI_GetTickCount();
    SCI_TRACE_LOW("[MMIDC] GetVideoThumbnailCallback t1=%d", t1);
        
    if(0 != image_data_ptr->coordinate.trim_w
       && 0 != image_data_ptr->coordinate.trim_h
       && img_info_ptr->img_width >= image_data_ptr->coordinate.trim_w
       && img_info_ptr->img_height >= image_data_ptr->coordinate.trim_h)
    {
        img_info_ptr->actual_width = image_data_ptr->coordinate.trim_w;
        img_info_ptr->actual_height = image_data_ptr->coordinate.trim_h;
            
        video_fullpathname = MMIAPIVP_ThumbnailGetVideo();
        MMI_WSTRNCPY(img_info_ptr->full_path_name,
                MMIDC_FULL_PATH_LENGTH, 
                video_fullpathname, 
                MMIAPICOM_Wstrlen((const wchar *) video_fullpathname), 
                MMIAPICOM_Wstrlen((const wchar *) video_fullpathname));        

        MMI_MEMCPY(img_info_ptr->img_data_buffer, 
                    img_info_ptr->img_data_size,
                    image_data_ptr->buf.oneplane.p_data, 
                    image_data_ptr->buf.oneplane.len,
                    image_data_ptr->buf.oneplane.len);
        
        MMIDC_SetLastThumbnailDataValidFlag(TRUE);
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    cmd_param.cmd = MMIDC_GetDCModuleCmd();
    MMK_PostMsg(win_id, MSG_DC_MMI_CMD, &cmd_param, sizeof(cmd_param));

    SCI_TRACE_LOW("[MMIDC] GetVideoThumbnailCallback result = %d", result);
    
    return result;
}
#endif

/*****************************************************************************/
//Description : get last thumbnail decoded data
//Global resource dependence : 
//Author: chunyou
//Note: 
/*****************************************************************************/
LOCAL MMIDC_GET_LAST_THUMBNAIL_DATA_E GetLastThumbnailDecodedData(CAMERA_MODE_E camera_mode,//in
                                                    GUIANIM_FILE_INFO_T  *file_info_ptr//in
                                                    )
{
    MMIDC_GET_LAST_THUMBNAIL_DATA_E get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
    BOOLEAN             result = FALSE;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    uint32 t1 = 0;
    uint32 t2 = 0;
    
    if (
        (PNULL == file_info_ptr)
        || (PNULL == file_info_ptr->full_path_wstr_ptr)
        || (0 == file_info_ptr->full_path_wstr_len)
        || (PNULL == img_info_ptr)
        || (PNULL == img_info_ptr->img_data_buffer)
        )
    {
        get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData Prame NULL !!!");
        return get_data_type;
    }

    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData camera_mode = %d", camera_mode);
    
    t1 = SCI_GetTickCount();
    if(CAMERA_MODE_DC == camera_mode)
    {
        //set image full path
        img_src.is_file = TRUE;
        img_src.full_path_ptr = file_info_ptr->full_path_wstr_ptr;
        
        //set image dest width and height
        img_display.dest_width = img_info_ptr->img_width;
        img_display.dest_height = img_info_ptr->img_height;

        //set image decode output data buffer
        img_output.decode_data_ptr = img_info_ptr->img_data_buffer;
        img_output.decode_data_size = img_info_ptr->img_data_size;        
        
        //decode jpg
        result = GUIIMG_Decode(&img_src,
            &img_display,
            &img_output);
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData GUIIMG_Decode result = %d", result);
        
        if (result
            && 0 != img_output.actual_width
            && 0 != img_output.actual_height
            && img_info_ptr->img_width >= img_output.actual_width
            && img_info_ptr->img_height >= img_output.actual_height)
        {
            img_info_ptr->actual_width = img_output.actual_width;
            img_info_ptr->actual_height = img_output.actual_height;
            MMI_WSTRNCPY(img_info_ptr->full_path_name,
                MMIDC_FULL_PATH_LENGTH, 
                file_info_ptr->full_path_wstr_ptr,
                file_info_ptr->full_path_wstr_len, 
                file_info_ptr->full_path_wstr_len); 
            
            MMIDC_SetLastThumbnailDataValidFlag(TRUE);
            get_data_type = MMIDC_GET_LAST_DATA_PHOTO;
        }
        else
        {
            get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
        }        
    }
    else
    {
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        VP_DISPLAY_PARAM_T  display_param = {0};
        GUI_RECT_T  display_rect = {0};
        
        display_param.audioSupport = 0;
        display_param.videoSupport = 1;
        display_param.b_support_seek = 1;
        display_param.disp_mode = DPLAYER_DISP_FULLVIEW;    
        display_param.RotateAngle = 0;
        display_param.disp_data_op = DPLAYER_DISP_DATA_GET;

        display_rect.right = img_info_ptr->img_width - 1;
        display_rect.bottom = img_info_ptr->img_height - 1;
        
        display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
        display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;       
        display_param.disp_rect.x = display_rect.left;
        display_param.disp_rect.y = display_rect.top;   

        display_param.target_rect.x = display_param.disp_rect.x;
        display_param.target_rect.y = display_param.disp_rect.y;
        display_param.target_rect.dx = display_param.disp_rect.dx;
        display_param.target_rect.dy = display_param.disp_rect.dy;

        MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);

#if 1
        result = MMIAPIVP_ThumbnailOpen(file_info_ptr->full_path_wstr_ptr, 
                                        MMIAPICOM_Wstrlen(file_info_ptr->full_path_wstr_ptr), 
                                        0,
                                        GetVideoThumbnailCallback, 
                                        &display_param,
                                        FALSE);
#endif
        if(result)
        {
            get_data_type = MMIDC_GET_LAST_DATA_VIDEO;
        }
        else
        {
            get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
        }
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData MMIAPIVP_ThumbnailOpen result = %d", result);
#endif

    }

    t2 = SCI_GetTickCount();
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData t2=%d, t2-t1=%d",
            t2, t2 - t1);
        
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData result = %d", result);
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailDecodedData get_data_type = %d", get_data_type);

    return (get_data_type);
}

/*****************************************************************************/
//Description : 在指定目录下搜索一个文件
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/ 
LOCAL BOOLEAN FindFirstImgFiles(
                                BOOLEAN  is_file,//in
                                wchar *find_path_ptr, //in
                                uint16 find_path_len, //in
                                wchar *filter_str_ptr, //in
                                MMI_STRING_T *file_name_ptr //out
                                )
{
    BOOLEAN ret = FALSE;
    SFS_HANDLE      file_handle = NULL;
    wchar           *find_ptr = PNULL;//改为malloc分配空间，解决堆栈溢出的问题
    SFS_FIND_DATA_T *find_data_ptr = PNULL;//改为malloc分配空间，解决堆栈溢出的问题
    uint16          ctlStr_d[] = L"/A:-H-S/D:-H-S";
    uint16          ctlStr_a[] = L"/A:-H-S";
    SFS_ERROR_E     result = SFS_NO_ERROR;
    
    if((PNULL == find_path_ptr) || (PNULL == filter_str_ptr))
    {
        //SCI_TRACE_LOW:"[MMIDC] FindFirstImgFiles param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_6121_112_2_18_2_9_4_282,(uint8*)"");
        return ret;
    }
    
    //alloc memory
    find_data_ptr = (SFS_FIND_DATA_T *)SCI_ALLOCA(sizeof(SFS_FIND_DATA_T));
    if(PNULL == find_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDC]:FindFirstImgFiles error:PNULL == find_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_6129_112_2_18_2_9_4_283,(uint8*)"");
        return ret;
    }
    SCI_MEMSET(find_data_ptr, 0, sizeof(SFS_FIND_DATA_T));
    
    find_ptr = (wchar *)SCI_ALLOCA( ( MMIFILE_FULL_PATH_MAX_LEN + 3 ) * sizeof(wchar) );
    if(PNULL == find_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDC]:FindFirstImgFiles error:PNULL == find"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_6137_112_2_18_2_9_5_284,(uint8*)"");
        
        SCI_FREE(find_data_ptr);
        return ret;
    }
    
    SCI_MEMSET(find_ptr, 0, (( MMIFILE_FULL_PATH_MAX_LEN + 3 ) * sizeof(wchar)) );
    
    //建立查找条件
    MMI_WSTRNCPY(find_ptr, MMIFILE_FULL_PATH_MAX_LEN, 
        find_path_ptr, find_path_len,
        find_path_len
        );
    
    //查找文件数据
    if (!is_file)//如何搜索不是只搜索文件or如果搜索是文件，但是需要搜索子目录文件
    { 
        result = SFS_FindFirstFileCtrl(find_ptr,filter_str_ptr,ctlStr_d,&file_handle,find_data_ptr,PNULL);
    }       
    else
    {
        result = SFS_FindFirstFileCtrl(find_ptr,filter_str_ptr,ctlStr_a,&file_handle,find_data_ptr,PNULL);
    }
    
    if (NULL != file_handle
        && SFS_NO_ERROR == result
        )
    {
        ret = TRUE;        
        //MMIAPICOM_Wstrncpy(file_name_ptr->wstr_ptr, find_data_ptr->name, file_name_ptr->wstr_len);
        MMI_WSTRNCPY(file_name_ptr->wstr_ptr, 
            file_name_ptr->wstr_len, 
            find_data_ptr->name, 
            MMIAPICOM_Wstrlen(find_data_ptr->name), 
            MMIAPICOM_Wstrlen(find_data_ptr->name));
        file_name_ptr->wstr_len = MMIAPICOM_Wstrlen(find_data_ptr->name);
    }
    
    if (NULL != file_handle)
    {
        SFS_FindClose(file_handle);
    }

    SCI_FREE(find_data_ptr);
    SCI_FREE(find_ptr);
    
    //SCI_TRACE_LOW:"[MMIDC] FindFirstImgFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIPDA_6178_112_2_18_2_9_5_285,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
//Description : Search first files
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchFirstFile(CAMERA_MODE_E mode,
                              MMI_STRING_T *file_name_ptr //out
                              )
{
    BOOLEAN ret = FALSE;
    wchar *path_ptr = PNULL;
    uint16 path_len = 0;
    wchar *filter_ptr = PNULL;
    MMI_STRING_T find_string = {0};
    wchar find_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint32 t1 = 0;
    uint32 t2 = 0;

    t1 = SCI_GetTickCount();
    //set search path
    if (CAMERA_MODE_DC == mode)
    {
        path_ptr = MMIDC_GetPhotoSavePath();
        path_len = (uint16)MMIAPICOM_Wstrlen(path_ptr);
        filter_ptr = (wchar*)s_photo_img_all_suffix;
    }
    else
    {
        path_ptr = MMIDC_GetVideoSavePath();
        path_len = (uint16)MMIAPICOM_Wstrlen(path_ptr);
        filter_ptr = (wchar*)s_video_img_all_suffix;
    }
    
    //search all the picture file
    find_string.wstr_ptr = find_file_name;
    find_string.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    if(PNULL != path_ptr) /*lint !e831*/
	{
		ret = FindFirstImgFiles(FALSE,
			path_ptr, 
			path_len,
			filter_ptr,
			&find_string
			);
	}
    
    if (ret)
    {
        file_name_ptr->wstr_len = MMIAPIFMM_CombineFullPathEx(file_name_ptr->wstr_ptr, 
            file_name_ptr->wstr_len,
            MMIAPIFMM_GetDeviceTypeByPath(path_ptr, MMIFILE_DEVICE_LEN),
            path_ptr + 3, 
            find_string.wstr_ptr
            ); /*lint !e613*/

    }
    t2 = SCI_GetTickCount();
    SCI_TRACE_LOW("[MMIDC] SearchFirstFile t2 = %d, t2-t1 = %d",
            t2, t2 - t1);
    SCI_TRACE_LOW("[MMIDC] SearchFirstFile ret = %d", ret);
    return ret;
}

/*****************************************************************************/
//Description :  get last thumbnail data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMIDC_GET_LAST_THUMBNAIL_DATA_E GetLastThumbnailData(CAMERA_MODE_E camera_mode)
{
    MMIDC_GET_LAST_THUMBNAIL_DATA_E get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
    BOOLEAN ret = FALSE;
    wchar name_wchar[MMIDC_FULL_PATH_LENGTH+1] = {0};
    MMI_STRING_T file_name = {0};
    GUIANIM_FILE_INFO_T file_info = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    BOOLEAN is_exist = FALSE;    

    if(PNULL == img_info_ptr)
    {
        return get_data_type;
    }

    if(img_info_ptr->is_img_data_valid)
    {
        is_exist = MMIAPIFMM_IsFileExist(img_info_ptr->full_path_name, MMIAPICOM_Wstrlen(img_info_ptr->full_path_name));
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailData is_exist = %d", is_exist);
        if(is_exist)
        {
            get_data_type = MMIDC_GET_LAST_DATA_BUFFER;
        }
    }

    if(!is_exist)
    {
        MMIDC_SetLastThumbnailDataValidFlag(FALSE);
        is_exist = ReadLastThumbnailDataFromFile(camera_mode);
    }
    
    if(!is_exist)
    {    
        MMIDC_SetLastThumbnailDataValidFlag(FALSE);
        
        file_name.wstr_len = MMIDC_FULL_PATH_LENGTH;
        file_name.wstr_ptr = name_wchar;
        //search photo
        ret = SearchFirstFile(camera_mode, &file_name);
        if(ret)
        {
            file_info.full_path_wstr_ptr = file_name.wstr_ptr;
            file_info.full_path_wstr_len = file_name.wstr_len;
            get_data_type = GetLastThumbnailDecodedData(camera_mode, &file_info);
        }        
    }
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailData ret = %d", ret);
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailData get_data_type = %d", get_data_type);

    return get_data_type;
}

/*****************************************************************************/
//Description :  get preview image data 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_GET_LAST_THUMBNAIL_DATA_E MMIDC_GetLastThumbnailData(CAMERA_MODE_E camera_mode
                                    )
{
    return GetLastThumbnailData(camera_mode);
}

/*****************************************************************************/
//Description :  clear last thumbnail rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ClearLastThumbnailRect(void)
{
    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

    if(PNULL != img_info_ptr)
    {
        MMIDC_ClearRect(img_info_ptr->img_display_rect, color);
    }
    SCI_TRACE_LOW("[MMIDC] ClearLastThumbnailRect");
}

/*****************************************************************************/
//Description :  get last thumbnail rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLastThumbnailRect(void)
{
    GUI_BOTH_RECT_T both_rect = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

    if(PNULL == img_info_ptr)
    {
        return both_rect.h_rect;
    }
    GetBtnPicBothRect(&both_rect);
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)    
    {
        img_info_ptr->img_display_rect = both_rect.h_rect;
    }
    else
    {
        img_info_ptr->img_display_rect = both_rect.v_rect;
    }
    img_info_ptr->img_display_rect.left += img_info_ptr->offset_x;
    img_info_ptr->img_display_rect.top += img_info_ptr->offset_y;
    img_info_ptr->img_display_rect.right -= img_info_ptr->offset_x;
    img_info_ptr->img_display_rect.bottom -= img_info_ptr->offset_y;

    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailRect left = %d, top = %d, right = %d, bottom = %d",
                    img_info_ptr->img_display_rect.left,
                    img_info_ptr->img_display_rect.top,
                    img_info_ptr->img_display_rect.right,
                    img_info_ptr->img_display_rect.bottom);
    
    return img_info_ptr->img_display_rect;
}

/*****************************************************************************/
//Description :  display last thumbnail
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayLastThumbnail(void)
{
    uint8 *data_ptr = PNULL;
    uint32              data_size = 0;
    GUI_LCD_DEV_INFO    lcd_dev = MMIDC_GetOSDBlockInfo();            //lcd dev
    GUI_RECT_T        display_rect={0};    
    UILAYER_COPYBUF_PARAM_T param = {0};
    UILAYER_COPYBUF_T copybuf = {0};
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

    if(PNULL == img_info_ptr
        || PNULL == img_info_ptr->img_data_buffer)
    {
        return;
    }
    
    data_ptr = img_info_ptr->img_data_buffer;
    data_size = img_info_ptr->img_data_size;
    display_rect = GetLastThumbnailRect();
    
    if(img_info_ptr->actual_width == img_info_ptr->img_width
        &&img_info_ptr->actual_height == img_info_ptr->img_height)
    {
        param.rect = display_rect;
    }
    else
    {
        LCD_FillRect(&lcd_dev, display_rect, MMI_BACKGROUND_COLOR);
        param.rect.left = display_rect.left + (img_info_ptr->img_width - img_info_ptr->actual_width)/2;
        param.rect.top = display_rect.top + (img_info_ptr->img_height - img_info_ptr->actual_height)/2;
        param.rect.right = display_rect.left + img_info_ptr->actual_width - 1;
        param.rect.bottom = display_rect.top + img_info_ptr->actual_height - 1;
    }
    copybuf.buf_ptr = data_ptr;
#if defined(MMIDC_F_PDA_BIT_16)
    copybuf.data_type = DATA_TYPE_RGB565;
#else
    copybuf.data_type = DATA_TYPE_ARGB888;
#endif
    copybuf.width = img_info_ptr->actual_width;
    copybuf.height = img_info_ptr->actual_height;
    
    UILAYER_CopyBufferToLayer(&lcd_dev,
        &param,
        &copybuf
        );
    SCI_TRACE_LOW("[MMIDC] DisplayLastThumbnail");
}
/*****************************************************************************/
//Description :  last thumbnail owner draw fun
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL void LastThumbnailOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

    if(PNULL == img_info_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[MMIDC] LastThumbnailOwnerDrawFun");
    if(img_info_ptr->is_img_data_valid)
    {
        DisplayLastThumbnail();
    }    
}

/*****************************************************************************/
//Description :  set last thumbnail data valid flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLastThumbnailDataValidFlag(BOOLEAN flag)
{
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

    if(PNULL != img_info_ptr)
    {
        img_info_ptr->is_img_data_valid = flag;
        SCI_TRACE_LOW("[MMIDC] MMIDC_SetLastThumbnailDataValidFlag flag = %d", img_info_ptr->is_img_data_valid);
    }      
}
#endif //MMIDC_F_LAST_IMAGE


#endif //MMIDC_DESKTOP_PDA
#endif //CAMERA_SUPPORT



/*Edit by script, ignore 3 case. Thu Apr 26 19:00:51 2012*/ //IGNORE9527


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
