/*****************************************************************************
** File Name:      mmiapwin_common.c                                         *
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

#define _MMIAPWIN_COMMON_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmi_appmsg.h"


//gui & mmk
#include "guilistbox.h"
#include "guirichtext.h"
#include "guilabel.h"
#include "guitext.h"
#include "guiedit.h"
#include "guistring.h"
#include "guiref_scale.h"
#include "guictrl_api.h"
#include "guires.h"
#include "guiform.h"
#include "guiownerdraw.h"
#include "guibutton.h"
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


//local module
#include "mmiap_position.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"

#include "mmiap_play.h"
#include "mmiap_setting.h"
#include "mmiapwin_set.h"
#include "mmimp3_export.h"
#include "mmiapwin_main.h"

#include "mmiap_list.h"
#include "mmiapwin_list.h"
#include "mmiap_list_control.h"

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

#include "mmiapwin_menu.h"
#include "mmiap_control.h"
#include "mmiapwin_common.h"
#include "mmiapwin_multilist.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_ALERT_DISPLAY_PERIOD     2000 //ms

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

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


LOCAL MMIAP_COMMON_WIN_INFO_T *s_mmiap_common_win_info_ptr = PNULL;
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get common win info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_COMMON_WIN_INFO_T *GetAPCommonWinInfoPtr(void)
{
    return s_mmiap_common_win_info_ptr;
}

/*****************************************************************************/
//  Description : create ap common win info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAPCommonWinInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_COMMON_WIN_INFO_T *info_ptr = GetAPCommonWinInfoPtr();
    uint32 info_size = sizeof(*info_ptr);


    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_common_win_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : destroy common win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyCommonWinInfo(
    void
)
{
    if (PNULL != s_mmiap_common_win_info_ptr)
    {
        SCI_FREE(s_mmiap_common_win_info_ptr);
    }
}

/*****************************************************************************/
//  Description : get ap button info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void GetAPButtonInfo(
    MMIAP_BUTTON_INFO_T *button_ptr //out
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (
        PNULL != button_ptr
        &&  PNULL != win_ptr
    )
    {
        *button_ptr = win_ptr->button_info;
    }
}

/*****************************************************************************/
//  Description : set ap button info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void SetAPButtonInfo(
    MMIAP_BUTTON_INFO_T *button_ptr //in
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (
        PNULL != button_ptr
        &&  PNULL != win_ptr
    )
    {
        win_ptr->button_info = *button_ptr;
    }
}

/*****************************************************************************/
//  Description : get glide ctrl head pointer
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T* GetAPGlideCtrlHeadPtr(
    void
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();
    MMI_CTRL_ID_T *ctrl_ptr = PNULL;

    if (
        PNULL != win_ptr
    )
    {
        ctrl_ptr = win_ptr->glide_ctrl;
    }


    return ctrl_ptr;
}

/*****************************************************************************/
//  Description : set anim ctrl id
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetAPAnimCtrlID(
    MMIAP_DIRECT_E which_ctrl, //in
    MMI_CTRL_ID_T ctrl_id //in
)
{
    MMI_CTRL_ID_T *head_ptr = GetAPGlideCtrlHeadPtr();


    if (
        PNULL != head_ptr
        && which_ctrl < MMIAP_DIRECT_MAX
    )
    {
        head_ptr[which_ctrl] = ctrl_id;
    }

    SCI_TRACE_LOW("[MMIAP] SetAPAnimCtrlID which_ctrl=%d, ctrl_id=%d",\
                  which_ctrl, ctrl_id);
}

/*****************************************************************************/
//  Description : get next album anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIAP_GetAnimCtrlID(
    MMIAP_DIRECT_E which_ctrl //in
)
{
    MMI_CTRL_ID_T *head_ptr = GetAPGlideCtrlHeadPtr();
    MMI_CTRL_ID_T ctrl_id = 0;


    if (
        PNULL != head_ptr
        && which_ctrl < MMIAP_DIRECT_MAX
    )
    {
        ctrl_id = head_ptr[which_ctrl];
    }


    SCI_TRACE_LOW("[MMIAP] MMIAP_GetAnimCtrlID which_ctrl=%d, ctrl_id=%d",\
                  which_ctrl, ctrl_id);

    return ctrl_id;
}

/*****************************************************************************/
//  Description : get alert info pointer
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_ALERT_INFO_T* GetAPAlertInfoPtr(
    void
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();
    MMIAP_ALERT_INFO_T *info_ptr = PNULL;


    if (
        PNULL != win_ptr
    )
    {
        info_ptr = &win_ptr->alert_info;
    }


    return info_ptr;
}

/*****************************************************************************/
//  Description : get alert Timer id
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetAPAlertTimerID(
    void
)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();
    uint8 timer_id = 0;



    if (PNULL != info_ptr)
    {
        timer_id = info_ptr->alert_timer;
    }


    SCI_TRACE_LOW("[MMIAP] GetAPAlertTimerID timer_id=%d",\
                  timer_id);

    return timer_id;
}

/*****************************************************************************/
//  Description : set alert timer id
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetAPAlertTimerID(
    uint8 timer_id //in
)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();


    if (
        PNULL != info_ptr
    )
    {
        info_ptr->alert_timer = timer_id;
    }
}
/*****************************************************************************/
//  Description : MMIMP3_GetProgressGrooveRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressGrooveRect(
    BOOLEAN is_bg_player,
    GUI_RECT_T *rect_ptr
)
{
    uint16 width = 0;
    uint16 height = 0;
    int16  y_offset = 0;
    static GUI_RECT_T s_rect = {0};
    static BOOLEAN s_is_horizon = FALSE;
    MMI_IMAGE_ID_T      img_id = IMAGE_COMMON_PLAYER_PROCESS_GROOVE;
    MMI_WIN_ID_T        win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID;

    if(PNULL == rect_ptr)
    {
        return;
    }

    if(GUI_IsRectEmpty(s_rect))
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();
    }

    if(GUI_IsRectEmpty(s_rect) || s_is_horizon != MMITHEME_IsMainScreenLandscape())
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();

        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        GUIOWNDRAW_GetDisplayRect(ctrl_id, &s_rect);

        y_offset = (s_rect.bottom - s_rect.top + 1 - height) / 2;

        //s_rect.left = s_rect.left + (s_rect.right - s_rect.left + 1 - width)/2;
        //s_rect.right = s_rect.left + width - 1;
        s_rect.left = s_rect.left + MMIMP3_PROGRESS_HORIZ_MARGIN;
        s_rect.right = s_rect.right - MMIMP3_PROGRESS_HORIZ_MARGIN;
        s_rect.top += y_offset;
        s_rect.bottom = s_rect.top + height - 1;
    }

    *rect_ptr = s_rect;
}

/*****************************************************************************/
//  Description : MMIMP3_GetProgressRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressRect(
    BOOLEAN is_bg_player,
    GUI_RECT_T *rect_ptr
)
{
    GUI_RECT_T groove_rect = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMI_IMAGE_ID_T  img_id = IMAGE_COMMON_PLAYER_PROCESS;
    static GUI_RECT_T s_rect = {0};
    static BOOLEAN s_is_horizon = FALSE;
    MMI_WIN_ID_T        win_id = MMIMP3_MAIN_PLAY_WIN_ID;

    if(PNULL == rect_ptr)
    {
        return;
    }

    if(GUI_IsRectEmpty(s_rect))
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();
    }

    if(GUI_IsRectEmpty(s_rect) || s_is_horizon != MMITHEME_IsMainScreenLandscape())
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();

        MMIMP3_GetProgressGrooveRect(is_bg_player, &groove_rect);
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);

        //s_rect.left = groove_rect.left + (groove_rect.right - groove_rect.left + 1 - width)/2;
        //s_rect.top  = groove_rect.top + (groove_rect.bottom - groove_rect.top + 1 - height)/2;
        //s_rect.right = s_rect.left + width - 1;
        s_rect.left  = groove_rect.left;
        s_rect.top   = groove_rect.top;
        s_rect.right = groove_rect.right;
        s_rect.bottom = s_rect.top + height - 1;
    }

    *rect_ptr = s_rect;
}

/*****************************************************************************/
//  Description : MMIMP3_GetProgressSlideWidthHeight
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetProgressSlideWidthHeight(
    uint16 *width_ptr,
    uint16 *height_ptr,
    MMI_WIN_ID_T win_id
)
{
    static uint16 s_width = 0;
    static uint16 s_height = 0;
    MMI_IMAGE_ID_T  img_id = IMAGE_COMMON_PLAYER_PROCESS_SLIDE;

    if(PNULL == width_ptr || PNULL == height_ptr)
    {
        return;
    }

    if(0 == s_width || 0 == s_height)
    {
        GUIRES_GetImgWidthHeight(&s_width, &s_height, img_id, win_id);
    }

    *width_ptr = s_width;
    *height_ptr = s_height;
}


/*****************************************************************************/
//  Description : get main anim ctrl vaild display rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMainAnimValidDisplayRect(GUI_RECT_T *rect_ptr)
{
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    uint16          up_osd_height = 0;
    uint16          down_osd_height = 0;

    GUIANIM_GetDisplayRect(ctrl_id, rect_ptr);

    //重新计算rect，去除OSD Panel区域
#ifndef MMI_RES_LOW_COST
    GUIRES_GetImgWidthHeight(PNULL, &up_osd_height, IMAGE_MP3_OSD_BG_UP, MMIMP3_MAIN_PLAY_WIN_ID);
#endif
    GUIRES_GetImgWidthHeight(PNULL, &down_osd_height, IMAGE_MP3_OSD_BG_DOWN, MMIMP3_MAIN_PLAY_WIN_ID);

    rect_ptr->top    += up_osd_height;
    rect_ptr->bottom -= down_osd_height;
}

/*****************************************************************************/
//  Description : get main anim ctrl vaild display rect for bg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMainAnimValidDisplayRectBg(GUI_RECT_T *rect_ptr)
{
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    uint16          up_osd_height = 0;
    uint16          down_osd_height = 0;

    GUIANIM_GetDisplayRect(ctrl_id, rect_ptr);

    if(MMIAP_IsOpenOsdPanel())
    {
        //重新计算rect，去除OSD Panel区域
#ifndef MMI_RES_LOW_COST
        GUIRES_GetImgWidthHeight(PNULL, &up_osd_height, IMAGE_MP3_OSD_BG_UP, MMIMP3_MAIN_PLAY_WIN_ID);
#endif
        GUIRES_GetImgWidthHeight(PNULL, &down_osd_height, IMAGE_MP3_OSD_BG_DOWN, MMIMP3_MAIN_PLAY_WIN_ID);
    }

    rect_ptr->top    += up_osd_height;
    rect_ptr->bottom -= down_osd_height;
}


/*****************************************************************************/
//  Description : get main anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetMainAnimCtrlId(void)
{
    return MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
}

/*****************************************************************************/
//  Description : get left anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetLeftAnimCtrlId(void)
{
    return MMIAP_GetAnimCtrlID(MMIAP_DIRECT_LEFT);
}

/*****************************************************************************/
//  Description : get right anim ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIMP3_GetRightAnimCtrlId(void)
{
    return MMIAP_GetAnimCtrlID(MMIAP_DIRECT_RIGHT);
}


/*****************************************************************************/
//  Description : release anim ctrl info
//  Global resource dependence :
//  Author: paul.huang
//  Note: 释放专辑封面ANIM控件占用的内存
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_ReleaseAnimCtrlInfo(void)
{
    MMI_CTRL_ID_T   left_ctrl_id  = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_LEFT);
    MMI_CTRL_ID_T   mid_ctrl_id   = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    MMI_CTRL_ID_T   right_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_RIGHT);
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUI_BG_T                err_bg = {0};

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = FALSE;

    err_bg.bg_type  = GUI_BG_NONE;
    //err_bg.img_id   = IMAGE_MP3_DEFAULT_ALBUM_IMG;
    GUIANIM_SetErrorBg(left_ctrl_id, &err_bg);
    GUIANIM_SetErrorBg(mid_ctrl_id, &err_bg);
    GUIANIM_SetErrorBg(right_ctrl_id, &err_bg);

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id    = left_ctrl_id;
    GUIANIM_SetParam(&ctrl_info, PNULL, PNULL, &display_info);
    ctrl_info.ctrl_id    = mid_ctrl_id;
    GUIANIM_SetParam(&ctrl_info, PNULL, PNULL, &display_info);
    ctrl_info.ctrl_id    = right_ctrl_id;
    GUIANIM_SetParam(&ctrl_info, PNULL, PNULL, &display_info);
}


/*****************************************************************************/
//  Description : handle review control.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3EqOrListButton(
    void
)
{
#ifdef MAINLCD_SIZE_128X64
#else
#ifdef MP3_EQ_SUPPORT
    /*
    if (MMIAPIBT_GetActiveBTHeadset())
    {
        MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_BT_NOT_SUPPORT_EQ,TXT_NULL,
            IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else*/
    {
        MMIAPSETWIN_OpenEQSetWin();
    }
#else
    MMIAPLISTWIN_OpenCurPlayList();
    //   MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_CUR_PLAY_LIST_WIN_TAB, PNULL);
#endif
#endif
}

/*****************************************************************************/
//  Description : EqBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_EqBtnCallback(void)
{
    HandleMp3EqOrListButton();
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : list button callback
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ListBtnCallback(void)
{
    //open current play list
    MMIAPLISTWIN_OpenCurPlayList();

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle repeat setting
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3RepeatButton(void)
{
    MMIMP3_REPEAT_MODE_E repeat_mode = APSETTING_GetRepeatMode();

    repeat_mode = (MMIMP3_REPEAT_MODE_E)((repeat_mode + 1) % MMIMP3_REPEAT_MAX);

    MMIMP3_SetRepeatMode(repeat_mode);

    //MMIAP_UpdateAnimCtrlInfo(TRUE);

    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        MMIMP3_DisplayRepeatButton(TRUE, FALSE, TRUE, MMIMP3_MAIN_PLAY_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : RepeatBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_RepeatBtnCallback(void)
{
    HandleMp3RepeatButton();

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Display mp3 repeat button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayRepeatButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
)
{
#ifndef MMI_RES_LOW_COST
    MMI_IMAGE_ID_T   repeat_image  = IMAGE_COMMON_BTN_REPEAT_GREY;
#else
    MMI_IMAGE_ID_T   repeat_image  = IMAGE_NULL;
#endif
    MMI_IMAGE_ID_T   press_repeat_img  = IMAGE_NULL;
    MMIMP3_REPEAT_MODE_E cur_mode  = MMIMP3_REPEAT_OFF;
    GUI_BG_T btn_bg = {0};
    GUI_BG_T press_fg = {0};

    cur_mode = is_bg_player ? APSETTING_GetRepeatMode() : (MMIMP3_REPEAT_OFF); //need further dvlp

    if(MMIMP3_IsButtonValid(MMIMP3_BUTTON_REPEAT))
    {
        switch(cur_mode)
        {
        case MMIMP3_REPEAT_OFF:
            repeat_image = IMAGE_COMMON_BTN_REPEAT_OFF;
#ifndef MMI_RES_LOW_COST
            press_repeat_img = IMAGE_COMMON_BTN_REPEAT_OFF_FOCUS;
#endif
            break;

        case MMIMP3_REPEAT_ONE:
            repeat_image = IMAGE_COMMON_BTN_REPEAT_ONE;
#ifndef MMI_RES_LOW_COST
            press_repeat_img = IMAGE_COMMON_BTN_REPEAT_ONE_FOCUS;
#endif
            break;

        case MMIMP3_REPEAT_ALL:
            repeat_image = IMAGE_COMMON_BTN_REPEAT_ON;
#ifndef MMI_RES_LOW_COST
            press_repeat_img = IMAGE_COMMON_BTN_REPEAT_ON_FOCUS;
#endif
            break;

        default:
            break;
        }
    }
    else
    {
#ifndef MMI_RES_LOW_COST
        repeat_image = IMAGE_COMMON_BTN_REPEAT_GREY;
#endif
    }

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = repeat_image;

    if(is_bg_player)
    {
        press_fg.bg_type = GUI_BG_IMG;
        press_fg.img_id  = press_repeat_img;
        GUIBUTTON_SetPressedFg(MMIMP3_REPEAT_BTN_CTRL_ID, &press_fg);
        GUIBUTTON_SetFg(MMIMP3_REPEAT_BTN_CTRL_ID, &btn_bg);

        if(is_update)
        {
            GUIBUTTON_Update(MMIMP3_REPEAT_BTN_CTRL_ID);
        }
    }

}

/*****************************************************************************/
//  Description : handle shffle setting
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3ShuffleButton(void)
{
    switch(APSETTING_GetShuffleMode())
    {
    case MMIMP3_SHUFFLE_ON:
        MMIMP3_SetShuffleMode(MMIMP3_SHUFFLE_OFF);
        break;

    case MMIMP3_SHUFFLE_OFF:
        MMIMP3_SetShuffleMode(MMIMP3_SHUFFLE_ON);

        break;

    default:
        break;
    }

    //MMIAP_UpdateAnimCtrlInfo(TRUE);

    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        MMIMP3_DisplayShuffleButton(TRUE, FALSE, TRUE, MMIMP3_MAIN_PLAY_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : ShuffleBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ShuffleBtnCallback(void)
{
    HandleMp3ShuffleButton();

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Display mp3 shuffle button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayShuffleButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
)
{
#ifndef MMI_RES_LOW_COST
    MMI_IMAGE_ID_T   shuffle_image = IMAGE_COMMON_BTN_SHUFFLE_GREY;
#else
    MMI_IMAGE_ID_T   shuffle_image = IMAGE_NULL;
#endif
    MMI_IMAGE_ID_T   press_img = IMAGE_NULL;
    MMIMP3_SHUFFLE_MODE_E cur_mode    = MMIMP3_SHUFFLE_OFF;
    GUI_BG_T btn_bg = {0};
    GUI_BG_T press_fg = {0};

    cur_mode = is_bg_player ? APSETTING_GetShuffleMode() : (MMIMP3_SHUFFLE_OFF); //need further dvlp

    if(MMIMP3_IsButtonValid(MMIMP3_BUTTON_SHUFFLE))
    {
        switch(cur_mode)
        {
        case MMIMP3_SHUFFLE_ON:
            shuffle_image = IMAGE_COMMON_BTN_SHUFFLE_ON;
#ifndef MMI_RES_LOW_COST
            press_img = IMAGE_COMMON_BTN_SHUFFLE_ON_FOCUS;
#endif
            break;

        case MMIMP3_SHUFFLE_OFF:
            shuffle_image = IMAGE_COMMON_BTN_SHUFFLE_OFF;
#ifndef MMI_RES_LOW_COST
            press_img = IMAGE_COMMON_BTN_SHUFFLE_OFF_FOCUS;
#endif
            break;

        default:
            break;
        }
    }
    else
    {
#ifndef MMI_RES_LOW_COST
        shuffle_image = IMAGE_COMMON_BTN_SHUFFLE_GREY;
#endif
    }

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = shuffle_image;

    if(is_bg_player)
    {
        press_fg.bg_type = GUI_BG_IMG;
        press_fg.img_id  = press_img;
        GUIBUTTON_SetPressedFg(MMIMP3_SHUFFLE_BTN_CTRL_ID, &press_fg);
        GUIBUTTON_SetFg(MMIMP3_SHUFFLE_BTN_CTRL_ID, &btn_bg);

        if(is_update)
        {
            GUIBUTTON_Update(MMIMP3_SHUFFLE_BTN_CTRL_ID);
        }
    }
}

/*****************************************************************************/
//  Description : PlayBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_PlayBtnCallback(void)
{
    MMIAP_HandleMp3PlayPauseButton();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle Play Pause setting
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleMp3PlayPauseButton(void)
{
    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
    {
        uint32 offset = MMIMP3_ZERO_OFFSET;


        offset = MMIAPIAP_GetCurPlayOffset(offset);

        MMIAPIMP3_PlayAudioPlayer(offset);
    }
    break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
    {
        switch(MMIMP3_GetAudioState())
        {
        case MMIMP3_AUDIO_PLAYING:
        	MMIMP3_GetPlayingDynInfo(PNULL, TRUE);
            MMIAPIMP3_PauseMp3(FALSE);
            break;

        case MMIMP3_AUDIO_PAUSED:
            MMIAPIMP3_ResumeMp3(FALSE);
            break;

        default:
            break;
        }
    }
    break;

    case MMIMP3_PLAYER_MUSIC_SWITCH:
        MMIAPIMP3_PauseMp3(FALSE);
        break;

    case MMIMP3_PLAYER_SWITCH_PAUSED:
        MMIAPIMP3_ResumeMp3(FALSE);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : PrevBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_PrevBtnCallback(void)
{
    MMIAPIMP3_GoToPreOrNextMp3(TRUE);

    MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, TRUE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : NextBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_NextBtnCallback(void)
{
    //SCI_TRACE_LOW:"[MMIMP3]:NextBtnCallback "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_2953_112_2_18_2_4_43_300, (uint8*)"");
    MMIAPIMP3_GoToPreOrNextMp3(FALSE);

    MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : set vol
// Global resource dependence :
// Author: xingdong.li
// Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(PNULL != para_ptr)
    {
        MMIAPIMP3_SetVolume(para_ptr->cur_value);

        MMIMP3_DisplayVolumeButton(TRUE, TRUE);
    }
}

/*****************************************************************************/
//  Description : Open Volume Panel.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenVolumePanel(
    void
)
{
    MMICOM_PANEL_INFO_T panel_info = {0};

    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.set_callback = SetVolumeCallback;
#ifndef UI_MULTILAYER_SUPPORT
    panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif
    MMIAPICOM_OpenPanelChildWin(MMIMP3_MAIN_PLAY_WIN_ID, &panel_info);
}

/*****************************************************************************/
//  Description : VolumeBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_VolumeBtnCallback(void)
{
    OpenVolumePanel();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Display mp3 volume images.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsVolumePanelOpen(
    void
)
{
    return MMIAPICOM_IsPanelOpened();
}

/*****************************************************************************/
//  Description : Display mp3 volume images.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CloseVolumePanel(
    void
)
{
    MMIAPICOM_ClosePanelChildWin();
}

/*****************************************************************************/
//  Description : return btn callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ReturnBtnCallback(void)
{
    MMK_CloseWin(MMIMP3_MAIN_PLAY_WIN_ID);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : OptBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_OptBtnCallback(void)
{
    MMIAP_OpenPlayerOptMenu(MMIMP3_MAIN_PLAY_WIN_ID);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DrawProgressOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    if(!MMIAP_IsOpenOsdPanel())
    {
        //osd panel not open
        return;
    }

    MMIMP3_DisplayProgress(TRUE);
}

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DrawTimeOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    if(!MMIAP_IsOpenOsdPanel())
    {
        //osd panel not open
        return;
    }

    MMIMP3_DisplayTimeInfo(TRUE);
}

/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMICOM_PANEL_INFO_T panel_info = {0};

    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.open_type = turn_vol_type;
    panel_info.set_callback = SetVolumeCallback;
#ifndef UI_MULTILAYER_SUPPORT
    panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinHandle(), &panel_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleSoftBarMsg(
    BOOLEAN is_update
)
{
#ifndef TOUCH_PANEL_SUPPORT
    MMI_WIN_ID_T    win_id = MMIMP3_MAIN_PLAY_WIN_ID;

    if(!MMK_IsFocusWin(win_id))
    {
        is_update = FALSE;
    }

    if(MMIAPIMP3_IsPlayerActive()
            && MMIAP_IsReviewOpen()
      )
    {
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

        switch(MMIMP3_GetReviewStatus())
        {
        case MMIMP3_REVIEW_STATE_NONE:
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_CANCEL, is_update);
            break;

        case MMIMP3_REVIEW_STATE_SET:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_STOP, TXT_NULL, STXT_CANCEL, is_update);
            break;

        case MMIMP3_REVIEW_STATE_PLAY:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, is_update);
            break;

        default:
            break;
        }

#endif
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, is_update);
    }

#endif
}

#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
/*****************************************************************************/
//  Description : MMIMP3_GetReviewSlideWidthHeight
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetReviewSlideWidthHeight(
    uint16 *width_ptr,
    uint16 *height_ptr,
    MMI_WIN_ID_T win_id
)
{
    static uint16 s_width = 0;
    static uint16 s_height = 0;

    MMI_IMAGE_ID_T  img_id = IMAGE_MP3_PROCESS_SLIDE_REVIEW;

    if(PNULL == width_ptr || PNULL == height_ptr)
    {
        return;
    }

    if(0 == s_width || 0 == s_height)
    {
        GUIRES_GetImgWidthHeight(&s_width, &s_height, img_id, win_id);
    }

    *width_ptr = s_width;
    *height_ptr = s_height;
}

/*****************************************************************************/
//  Description : Display mp3 review button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayReviewButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
)
{
    GUI_BG_T btn_bg = {0};
    GUI_BG_T press_fg = {0};
    BOOLEAN is_show = FALSE;

#ifdef MP3_SEEK_SUPPORT
#if !defined(MMI_RES_LOW_COST) && defined(MMI_AUDIOPLAYER_REVIEW_SUPPORT)
    {
        MMI_IMAGE_ID_T   review_image   = IMAGE_MP3_BTN_REVIEW_GREY;
        MMI_IMAGE_ID_T   press_review_img = IMAGE_NULL;
        MMIMP3_REVIEW_STATE_TYPE_E review_state = MMIMP3_REVIEW_STATE_NONE;

        if(MMIMP3_IsButtonValid(MMIMP3_BUTTON_REVIEW_OR_DETAIL))
        {
            review_state = MMIMP3_GetReviewStatus(); //need further dvlp

            switch(review_state)
            {
            case MMIMP3_REVIEW_STATE_NONE:
                review_image = IMAGE_MP3_BTN_REVIEW;
                press_review_img = IMAGE_MP3_BTN_REVIEW_FOCUS;
                break;

            case MMIMP3_REVIEW_STATE_SET:
                review_image = IMAGE_MP3_BTN_REVIEW_SET;
                press_review_img = IMAGE_MP3_BTN_REVIEW_SET_FOCUS;
                break;

            case MMIMP3_REVIEW_STATE_PLAY:
                review_image = IMAGE_MP3_BTN_REVIEW_PLAY;
                press_review_img = IMAGE_MP3_BTN_REVIEW_PLAY_FOCUS;
                break;

            default:
                break;
            }
        }
        else
        {
            review_image = IMAGE_MP3_BTN_REVIEW_GREY;
        }

        btn_bg.bg_type = GUI_BG_IMG;
        btn_bg.img_id  = review_image;

        press_fg.bg_type = GUI_BG_IMG;
        press_fg.img_id  = press_review_img;

        is_show = TRUE;
    }
#endif //#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT   
#else //#ifdef MP3_SEEK_SUPPORT
    {
        //no define MP3_SEEK_SUPPORT
        MMI_IMAGE_ID_T      rev_or_dtl_img  = MMIMP3_IsButtonValid(MMIMP3_BUTTON_REVIEW_OR_DETAIL) ? IMAGE_COMMON_BTN_DETAIL : IMAGE_COMMON_BTN_DETAIL_GREY;


        btn_bg.bg_type = GUI_BG_IMG;
        btn_bg.img_id  = rev_or_dtl_img;
        press_fg = btn_bg;

        is_show = TRUE;
    }
#endif

    if (is_show)  /*lint !e774*/
    {
        GUIBUTTON_SetPressedFg(MMIMP3_REVIEW_BTN_CTRL_ID, &press_fg);
        GUIBUTTON_SetFg(MMIMP3_REVIEW_BTN_CTRL_ID, &btn_bg);

        if(is_update)
        {
            GUIBUTTON_Update(MMIMP3_REVIEW_BTN_CTRL_ID);
        }
    }
}

/*****************************************************************************/
//  Description : handle review control.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleMp3ReviewButton(
    void
)
{
    MMIMP3_HandleMp3ReviewPlay();

    if(MMIMP3_REVIEW_STATE_NONE != MMIMP3_GetReviewStatus())
    {
        MMIAP_SetReviewOpenFlag(TRUE);
    }
    else
    {
        MMIAP_SetReviewOpenFlag(FALSE);
    }

    MMIAP_UpdateReviewMenuStatus();
    MMIAP_HandleSoftBarMsg(TRUE);

    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        MMIMP3_DisplayReviewButton(TRUE, TRUE, TRUE, MMIMP3_MAIN_PLAY_WIN_ID);
        //MMIMP3_DisplayButton(TRUE);
        MMIMP3_DisplayProgressAndTimeInfo(TRUE);
    }
}

/*****************************************************************************/
//  Description : ReviewBtnCallback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_ReviewBtnCallback(void)
{
    MMIAP_HandleMp3ReviewButton();


    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : set review open flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReviewOpenFlag(
    BOOLEAN is_open //in
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (PNULL != win_ptr)
    {
        win_ptr->is_review_open = is_open;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SetReviewOpenFlag is_open=%d",\
                  is_open);
}

/*****************************************************************************/
//  Description : review open flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsReviewOpen(
    void
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();
    BOOLEAN is_open = FALSE;


    if (PNULL != win_ptr)
    {
        is_open = win_ptr->is_review_open;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_IsReviewOpen is_open=%d",\
                  is_open);

    return is_open;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleReviewMenu(
    void
)
{
    if(!MMIMP3_IsCurFileSeekSupport())
    {
        //SCI_TRACE_LOW:"[MMIMP3] MMIAP_HandleReviewMenu seek not supportted"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4336_112_2_18_2_4_45_303, (uint8*)"");
        return;
    }

    MMIAP_UpdateReviewMenuStatus();

    if(MMIAP_IsReviewOpen())
    {
        MMIAP_SetReviewOpenFlag(FALSE);
        MMIMP3_ExitReview();
    }
    else
    {
        if(MMIAPIMP3_IsPlayerActive())
        {
            MMIAP_HandleMp3ReviewButton();
        }
    }

    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        MMIMP3_DisplayButton(TRUE, TRUE);
    }

    MMIAP_HandleSoftBarMsg(FALSE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearReviewMenu(
    void
)
{
    MMIAP_SetReviewOpenFlag(FALSE);

    MMIAP_HandleSoftBarMsg(FALSE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateReviewMenuStatus(
    void
)
{
    if(MMIMP3_REVIEW_STATE_NONE != MMIMP3_GetReviewStatus())
    {
        MMIAP_SetReviewOpenFlag(TRUE);
    }
}

#endif //#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT


/*****************************************************************************/
//  Description : Init mp3 button valid status.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMp3ButtonValidStatus(
    void
)
{
    MMIAP_BUTTON_INFO_T button_info = {0};
    uint32 i = 0;


    GetAPButtonInfo(&button_info);

    for(i = 0; i < MMIMP3_BUTTON_MAX; i++)
    {
        button_info.button_valid[i] = TRUE;
    }

    SetAPButtonInfo(&button_info);
}

/*****************************************************************************/
//  Description : UpdateBtnCtrlStatus
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateBtnCtrlStatus(
    BOOLEAN is_bg_player
)
{
    MMIAP_BUTTON_INFO_T button_info = {0};
    uint32 i = 0;
    BOOLEAN is_gray = FALSE;


    if(is_bg_player)
    {

        GetAPButtonInfo(&button_info);

        for(i = 0; i < MMIMP3_BUTTON_MAX; i++)
        {
            is_gray = !button_info.button_valid[i];

            switch(i)
            {
            case MMIMP3_BUTTON_SHUFFLE:
                GUIBUTTON_SetGrayed(MMIMP3_SHUFFLE_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            case MMIMP3_BUTTON_REPEAT:
                GUIBUTTON_SetGrayed(MMIMP3_REPEAT_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;
#ifdef TOUCH_PANEL_SUPPORT

            case MMIMP3_BUTTON_EQ_OR_LIST:
                GUIBUTTON_SetGrayed(MMIMP3_EQ_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;
#endif

            case MMIMP3_BUTTON_VOLUME_SET:
                GUIBUTTON_SetGrayed(MMIMP3_VOLUME_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            case MMIMP3_BUTTON_PREV:
                GUIBUTTON_SetGrayed(MMIMP3_PREV_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            case MMIMP3_BUTTON_PLAY_PAUSE:
                GUIBUTTON_SetGrayed(MMIMP3_PLAY_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            case MMIMP3_BUTTON_NEXT:
                GUIBUTTON_SetGrayed(MMIMP3_NEXT_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

            case MMIMP3_BUTTON_REVIEW_OR_DETAIL:
                GUIBUTTON_SetGrayed(MMIMP3_REVIEW_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;
#endif

            case MMIMP3_BUTTON_LEFT_KEY:
                GUIBUTTON_SetGrayed(MMIMP3_OPT_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            case MMIMP3_BUTTON_RETURN:
                GUIBUTTON_SetGrayed(MMIMP3_RETRUN_BTN_CTRL_ID, is_gray, PNULL, FALSE);
                break;

            default:
                break;
            }
        }
    }

}

/*****************************************************************************/
//  Description : Update mp3 button valid status.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_UpdateButtonValidStatus(
    BOOLEAN is_bg_player
)
{
    MMIAP_BUTTON_INFO_T button_info = {0};
    uint32 total_num = 0;


    InitMp3ButtonValidStatus();

    if(is_bg_player)
    {
        GetAPButtonInfo(&button_info);

        switch(MMIAPIMP3_GetPlayerState())
        {
        case MMIMP3_PLAYER_STATE_NONE:
        {
            BOOLEAN is_valid = FALSE;


            is_valid = button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL];

#ifdef MP3_SEEK_SUPPORT
            is_valid = FALSE;
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST

            if(MMIAP_IsHistoryInit())
            {
                is_valid = MMIAP_GetReviewButtonStatusFromLastInfo();
            }

#endif


            SCI_TRACE_LOW("[MMIMP3] MMIMP3_UpdateButtonValidStatus review is_valid=%d",
                          is_valid);

            button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL] = is_valid;
        }
        break;

        case MMIMP3_PLAYER_AUDIO_ACTIVE:
            switch(MMIMP3_GetAudioState())
            {
            case MMIMP3_AUDIO_PLAYING:
            case MMIMP3_AUDIO_PAUSED:
#ifdef MP3_SEEK_SUPPORT
                if(!MMIMP3_IsCurFileSeekSupport())
                {
                    button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL] = FALSE;
                }

#endif
                break;

            case MMIMP3_AUDIO_STOPPED:
#ifdef MP3_SEEK_SUPPORT
                button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL] = FALSE;
#endif
                break;

            default:
                break;
            }

            break;

        case MMIMP3_PLAYER_MUSIC_SWITCH:
        case MMIMP3_PLAYER_SWITCH_PAUSED:
            if(MMIMP3_REVIEW_STATE_NONE == MMIMP3_GetReviewStatus())
            {
#ifdef MP3_SEEK_SUPPORT
                button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL] = FALSE;
#endif
            }

            break;

        default:
            break;
        }

        total_num = MMIMP3_GetCurPlayListTotalNum();
        if(0 == total_num)
        {
            button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL] = FALSE;
            button_info.button_valid[MMIMP3_BUTTON_PREV] = FALSE;
            button_info.button_valid[MMIMP3_BUTTON_NEXT] = FALSE;
            button_info.button_valid[MMIMP3_BUTTON_PLAY_PAUSE] = FALSE;
        }
        else if(1 == total_num && !MMIMP3_IsCurFileSeekSupport())
        {
            button_info.button_valid[MMIMP3_BUTTON_PREV] = FALSE;
            button_info.button_valid[MMIMP3_BUTTON_NEXT] = FALSE;
        }

        SetAPButtonInfo(&button_info);
    }

    UpdateBtnCtrlStatus(is_bg_player);

}

/*****************************************************************************/
//  Description : Judge whether a button is valid .
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsButtonValid(
    MMIMP3_BUTTON_TYPE_E button
)
{
    MMIAP_BUTTON_INFO_T button_info = {0};
    BOOLEAN is_valid = FALSE;


    if(button < MMIMP3_BUTTON_MAX)
    {
        GetAPButtonInfo(&button_info);
        is_valid = button_info.button_valid[button];
    }

    return is_valid;
}

#ifdef MMIAP_F_HISTORY_PLAYLIST
/*****************************************************************************/
//  Description : get history panel info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetHistoryPanelInfo(MMIAP_HISTORY_PANEL_INFO_T *info_ptr //out
                                     )
{
    MMIAP_BUTTON_INFO_T button_info = {0};

    if(PNULL == info_ptr)
    {
        return ;
    }

    GetAPButtonInfo(&button_info);
    info_ptr->is_review_button_focus = button_info.button_valid[MMIMP3_BUTTON_REVIEW_OR_DETAIL];
}

#endif

/*****************************************************************************/
//  Description : open mp3 pda osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_OpenMp3OsdPanel(
    BOOLEAN is_update
)
{
    //MMI_STRING_T tip_text = {0};
    //wchar        tip_wstr[MMIMP3_DISPLAY_TIME_LEN +1] = {0};

    if(MMIAP_IsOpenOsdPanel())
    {
        //SCI_TRACE_LOW:"[MMIMP3]:OpenMp3OsdPanel is already opened!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_17660_112_2_18_2_5_15_360, (uint8*)"");
        return;
    }

    //set flag
    MMIAP_SetOsdPanelFlag(TRUE);


    GUILABEL_SetVisible(MMIMP3_FILE_INFO_LABLE_CTRL_ID, TRUE, is_update);
    GUIFORM_SetVisible(MMIMP3_OSD_PANEL_FORM_CTRL_ID, TRUE, is_update);

    //update file tip info
    MMIAP_UpdateOsdFileTip(is_update);

    //display progress and time info
    MMIMP3_DisplayProgressAndTimeInfo(TRUE);
}

/*****************************************************************************/
//  Description : close mp3 pda osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CloseMp3OsdPanel(
    BOOLEAN is_update
)
{
    //up panel
    GUILABEL_SetVisible(MMIMP3_FILE_INFO_LABLE_CTRL_ID, FALSE, is_update);
    //down panel
    GUIFORM_SetVisible(MMIMP3_OSD_PANEL_FORM_CTRL_ID, FALSE, is_update);

    //set flag
    MMIAP_SetOsdPanelFlag(FALSE);

#ifdef MMIAP_LYRIC_SUPPORT
    if ((MMIMP3_DISPLAY_LYRIC_ON == APSETTING_GetDisplayStyle()
            || MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle())
            && (MMIMP3_IsLyricExist()))
    {
        MMK_SendMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_MP3_LYRIC_DISPLAY, PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : whether mp3 pda osd panel opened
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsOpenOsdPanel(void)
{
    BOOLEAN is_osd_panel_open = FALSE;
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (PNULL != win_ptr)
    {
        is_osd_panel_open = win_ptr->is_osd_panel_open;
    }

    return is_osd_panel_open;
}

/*****************************************************************************/
//  Description : set osd panel open flag
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetOsdPanelFlag(BOOLEAN is_open)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (PNULL != win_ptr)
    {
        win_ptr->is_osd_panel_open = is_open;
    }
}

/*****************************************************************************/
//  Description : get file tip info for osd up panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetOsdFileTipInfo(wchar   *wstr_ptr,
                                       uint16  wstr_len
                                      )
{
    char    str_tip[MMIMP3_DISPLAY_TIME_LEN + 1] = {0};
    wchar   wstr_tip[MMIMP3_DISPLAY_TIME_LEN + 1] = {0};
    uint16  tip_len = 0;
    uint16  min_len = 0;
    uint32  index = MMIMP3_GetCurPlayFileIndex();
    uint32  total_num = MMIMP3_GetCurPlayListTotalNum();

    if(PNULL != wstr_ptr
            && wstr_len > 0)
    {
        if(total_num > 0)
        {
            //索引从0开始
            index ++;
        }

        sprintf(str_tip, "%d/%d", index, total_num);
        MMIAPICOM_StrToWstr((uint8*)str_tip, wstr_tip);
        tip_len = MMIAPICOM_Wstrlen(wstr_tip);

        min_len = MIN(tip_len, wstr_len);
        MMIAPICOM_Wstrncpy(wstr_ptr, wstr_tip, min_len);

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : update osd file tip
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateOsdFileTip(
    BOOLEAN is_update
)
{
    MMI_STRING_T tip_text = {0};
    wchar        tip_wstr[MMIMP3_DISPLAY_TIME_LEN + 1] = {0};

    if(!MMIAP_IsOpenOsdPanel())
    {
        //SCI_TRACE_LOW:"[MMIMP3]:OpenMp3OsdPanel osd panel not open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_17760_112_2_18_2_5_15_361, (uint8*)"");
        return;
    }

#ifdef BT_AV_CT_SUPPORT
	if(MMIAPIMp3_IsBtMusic())
	{
		UpdateAvrcpPlayerDisplay();
		GUILABEL_SetFont(MMIMP3_FILE_INFO_LABLE_CTRL_ID, MMIMP3_SUB_TEXT_FONT, MMI_WHITE_COLOR);
		return;
	}
#endif

    //get file tip info
    MMIAP_GetOsdFileTipInfo(tip_wstr, MMIMP3_DISPLAY_TIME_LEN);
    tip_text.wstr_ptr = tip_wstr;
    tip_text.wstr_len = MMIAPICOM_Wstrlen(tip_wstr);
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
    GUITITLE_SetSubText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), TRUE, tip_text.wstr_ptr, tip_text.wstr_len, is_update);
#else
    GUILABEL_SetText(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &tip_text, is_update);
#endif

#ifdef MAINLCD_SIZE_320X480
    GUILABEL_SetFont(MMIMP3_FILE_INFO_LABLE_CTRL_ID, MMIMP3_MAIN_TEXT_FONT, MMI_WHITE_COLOR);
#else
    GUILABEL_SetFont(MMIMP3_FILE_INFO_LABLE_CTRL_ID, MMIMP3_SUB_TEXT_FONT, MMI_WHITE_COLOR);
#endif
}

/*****************************************************************************/
//  Description : set osd panel reopen flag
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReopenOsdPanelFlag(
    BOOLEAN is_reopen
)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (PNULL != win_ptr)
    {
        win_ptr->is_reopen_osd_panel = is_reopen;
    }
}

/*****************************************************************************/
//  Description : is need reopen osd panel
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsNeedReopenOsdPanel(void)
{
    BOOLEAN is_reopen_osd_panel = FALSE;
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();


    if (PNULL != win_ptr)
    {
        is_reopen_osd_panel = win_ptr->is_reopen_osd_panel;
    }

    return is_reopen_osd_panel;
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : MMIAP_SetOpenMp3Entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetOpenMp3Entry(MMIMP3_ENTRY_TYPE_E entry_type)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();

    if (PNULL != win_ptr)
    {
        win_ptr->entry_type = entry_type;
    }
}

/*****************************************************************************/
//  Description : MMIAP_GetOpenMp3Entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_ENTRY_TYPE_E MMIAP_GetOpenMp3Entry(void)
{
    MMIAP_COMMON_WIN_INFO_T *win_ptr = GetAPCommonWinInfoPtr();
    MMIMP3_ENTRY_TYPE_E entry_type = MMIMP3_ENTRY_TYPE_MAX;


    if (PNULL != win_ptr)
    {
        entry_type = win_ptr->entry_type;
    }

    return entry_type;
}
#endif //#ifdef CMCC_UI_STYLE

/*****************************************************************************/
//  Description : set anim ctrl rect and display rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAnimCtrlRectDisplayRect(
    int16         move_x,
    MMI_CTRL_ID_T ctrl_id
)
{
    GUI_RECT_T      ctrl_rect = {0};
    GUI_RECT_T      full_rect_h = MMIMP3_ALBUM_ANIM_RECT_H;
    GUI_RECT_T      full_rect_v = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      display_rect = {0};

    //get control rect
    GUIAPICTRL_GetRect(ctrl_id, &ctrl_rect);

    ctrl_rect.left  = (int16)(ctrl_rect.left + move_x);
    ctrl_rect.right = (int16)(ctrl_rect.right + move_x);

    //set control rect
    GUIANIM_SetCtrlRect(ctrl_id, &ctrl_rect);

    //get display rect
    display_rect = MMITHEME_IsMainScreenLandscape() ? full_rect_h : full_rect_v;

    if(!GUI_IntersectRect(&display_rect, ctrl_rect, display_rect))
    {
        display_rect.left   = 0;
        display_rect.top    = 0;
        display_rect.right  = 0;
        display_rect.bottom = 0;
    }

    //set display rect
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, TRUE);
}

/*****************************************************************************/
//  Description : set pre album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetLeftAnimRect(BOOLEAN  is_update)
{
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_LEFT);
    GUI_RECT_T      display_rect = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    uint16          ver_album_width = 0;
    uint16          hor_album_width = 0;
    GUI_RECT_T      ver_album_rect = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      hor_album_rect = MMIMP3_ALBUM_ANIM_RECT_H;

    ver_album_width  = ver_album_rect.right - ver_album_rect.left + 1;
    hor_album_width  = hor_album_rect.right - hor_album_rect.left + 1;

    //ver rect of both rect
    both_rect.v_rect.right  = -1;
    both_rect.v_rect.left   = (int16)(both_rect.v_rect.right - ver_album_width + 1);
    both_rect.v_rect.top    = ver_album_rect.top;
    both_rect.v_rect.bottom = ver_album_rect.bottom;

    //hor rect of both rect
    both_rect.h_rect.right  = -1;
    both_rect.h_rect.left   = (int16)(both_rect.h_rect.right - hor_album_width + 1);
    both_rect.h_rect.top    = hor_album_rect.top;
    both_rect.h_rect.bottom = hor_album_rect.bottom;

    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set main album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetMidAnimRect(BOOLEAN  is_update)
{
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    GUI_RECT_T      display_rect = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T      ver_album_rect = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      hor_album_rect = MMIMP3_ALBUM_ANIM_RECT_H;

    both_rect.v_rect = ver_album_rect;
    both_rect.h_rect = hor_album_rect;

    if(MMITHEME_IsMainScreenLandscape())
    {
        display_rect = hor_album_rect;
    }
    else
    {
        display_rect = ver_album_rect;
    }

    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set next album anim rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetRightAnimRect(BOOLEAN  is_update)
{
    //MMI_WIN_ID_T    win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_RIGHT);
    GUI_RECT_T      display_rect = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    //uint16          hor_panel_width = MMIMP3_HOR_PANEL_WIDTH;
    uint16          ver_album_width = 0;
    uint16          hor_album_width = 0;
    GUI_RECT_T      ver_album_rect = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      hor_album_rect = MMIMP3_ALBUM_ANIM_RECT_H;

    ver_album_width  = ver_album_rect.right - ver_album_rect.left + 1;
    hor_album_width  = hor_album_rect.right - hor_album_rect.left + 1;

    //ver rect of both rect
    both_rect.v_rect.left   = ver_album_width;
    both_rect.v_rect.right  = (int16)(both_rect.v_rect.left + ver_album_width - 1);
    both_rect.v_rect.top    = ver_album_rect.top;
    both_rect.v_rect.bottom = ver_album_rect.bottom;

    //hor rect of both rect
    both_rect.h_rect.left   = hor_album_width; //+ hor_panel_width;
    both_rect.h_rect.right  = (int16)(both_rect.h_rect.left + hor_album_width - 1);
    both_rect.h_rect.top    = hor_album_rect.top;
    both_rect.h_rect.bottom = hor_album_rect.bottom;

    GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set anim ctrl param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SetAnimParam(
    MMI_CTRL_ID_T       ctrl_id,    //in
    MMIMP3_ALBUM_INFO_T *album_ptr, //in
    BOOLEAN             is_update
)
{
    GUI_BG_T                err_bg = {0};
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    //SCI_TRACE_LOW:"[MMIMP3] SetAnimParam addr:0x%x  size%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18486_112_2_18_2_5_16_386, (uint8*)"dd", album_ptr->img_data_ptr, album_ptr->data_size);

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id    = ctrl_id;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = is_update;

    //设置ANIM背景, 避免GUIANIM_UpdateRect刷新死循环
    display_info.bg.bg_type = GUI_BG_IMG;
    display_info.bg.img_id  = IMAGE_MP3_ALBUM_BG;

    if(PNULL != album_ptr
            && PNULL != album_ptr->img_data_ptr
            && album_ptr->data_size > 0)
    {
        display_info.is_crop_rect  = TRUE;   //允许裁剪图片
        display_info.is_syn_decode = TRUE;   //@test
        display_info.is_disp_one_frame = TRUE;
        display_info.is_zoom = TRUE;
        display_info.is_auto_zoom_in = TRUE;

        data_info.data_ptr  = album_ptr->img_data_ptr;
        data_info.data_size = album_ptr->data_size;

        //图片格式可能不支持或数据已损坏，使用默认专辑图片作为错误背景
        err_bg.bg_type  = GUI_BG_IMG;
        err_bg.img_id   = IMAGE_MP3_DEFAULT_ALBUM_IMG;
        GUIANIM_SetErrorBg(ctrl_id, &err_bg);
    }
    else
    {
        //无专辑图片，使用默认专辑图片
        data_info.img_id = IMAGE_MP3_DEFAULT_ALBUM_IMG;
    }

    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
}

/*****************************************************************************/
//  Description : set anim ctrl default param
//  Global resource dependence :
//  Author: xin.li
//  Note: 非专辑显示模式的默认背景设置
/*****************************************************************************/
LOCAL void SetAnimDefaultParam(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_update
)
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id    = ctrl_id;



    data_info.img_id = IMAGE_MP3_DEFAULT_ALBUM_IMG;


    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = is_update;

    //必须设置ANIM背景,否则GUIANIM_UpdateRect刷新导致死循环
    display_info.bg.bg_type = GUI_BG_IMG;
    display_info.bg.img_id  = IMAGE_MP3_ALBUM_BG;

    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
}

/*****************************************************************************/
//  Description : set anim ctrl param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAnimCtrlParam(
    MMIAP_DIRECT_E which_ctrl, //in
    BOOLEAN is_update
)
{
    MMI_CTRL_ID_T   ctrl_id = MMIAP_GetAnimCtrlID(which_ctrl);
    MMIMP3_ALBUM_INFO_T *album_info_ptr = PNULL;


#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    album_info_ptr = GetAppointedAlbumInfoPtr(which_ctrl);
#endif
    SetAnimParam(ctrl_id, album_info_ptr, is_update);
}

/*****************************************************************************/
//  Description : 显示歌手名、专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayAlbumTitleInfo(BOOLEAN is_update)
{
    MMI_STRING_T            text_artist = {0};
    MMI_STRING_T            text_album = {0};
    MMIMP3_ALBUM_INFO_T     *album_info_ptr = PNULL;


	SCI_TRACE_LOW("[MMIMP3]:MMIAP_DisplayAlbumTitleInfo, is_update =%d", is_update);

    //file name
    MMIMP3_DisplayFileName(is_update);


#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    album_info_ptr = GetAppointedAlbumInfoPtr(MMIAP_DIRECT_MID);
#endif

    if (PNULL != album_info_ptr)
    {
        //artist name
        text_artist.wstr_ptr = album_info_ptr->artist;
        text_artist.wstr_len = MMIAPICOM_Wstrlen(album_info_ptr->artist);
        GUILABEL_SetText(MMIMP3_ARTIST_LABLE_CTRL_ID, &text_artist, is_update);


        //album name
        text_album.wstr_ptr = album_info_ptr->album;
        text_album.wstr_len = MMIAPICOM_Wstrlen(album_info_ptr->album);
        GUILABEL_SetText(MMIMP3_ALBUM_LABLE_CTRL_ID, &text_album, is_update);
    }

#ifdef BT_AV_CT_SUPPORT
	if(MMIAPIMp3_IsBtMusic())
	{
		UpdateAvrcpPlayerDisplay();
	}
#endif
}

/*****************************************************************************/
//  Description : anim owner draw call back function
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void MainAnimOwnerDrawCallback(
    GUIANIM_OWNER_DRAW_T   *owner_draw_ptr
)
{
#ifdef MMIAP_LYRIC_SUPPORT

    if((MMIMP3_DISPLAY_LYRIC_ON == APSETTING_GetDisplayStyle()
            || MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle())
            && MMIMP3_IsLyricExist())
    {
        //display lyric
        MMIMP3_DisplayLyricImmediately();
    }


#endif
#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_DisplaySpectrum(MMIMP3_GetMainAnimCtrlId());
#endif
    //title anim
    MMIMP3_PlayTitleAnim(TRUE);

    MMIMP3_DisplayAlertInfo();
}

/*****************************************************************************/
//  Description : update album anim ctrl info
//  Global resource dependence :
//  Author: xin.li
//  Note: 更新专辑图片信息
//        无专辑图片信息时默认专辑图片，非专辑显示模式时使用默认背景图片
/*****************************************************************************/
PUBLIC void MMIAP_UpdateAnimCtrlInfo(
    BOOLEAN is_update
)
{
    MMI_CTRL_ID_T   mid_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    //get pre/cur/next album info
    MMIAP_GetAllAlbumInfo();

    if(MMIMP3_DISPLAY_ALBUM == APSETTING_GetDisplayStyle())
    {
        //set album info
        MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_LEFT, is_update);
        MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_MID, is_update);
        MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_RIGHT, is_update);
    }
    else
#endif
    {
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
        MMIAP_ReleaseAllAlbumImgInfo();
#endif

        //非专辑显示模式，使用默认背景
        SetAnimDefaultParam(mid_ctrl_id, is_update);

        //set ownerdraw callback
        GUIANIM_SetOwnerDraw(mid_ctrl_id, MainAnimOwnerDrawCallback);
    }

    //update artist and album name title display
    MMIAP_DisplayAlbumTitleInfo(is_update);

    //update up osd file tip
    MMIAP_UpdateOsdFileTip(is_update);
}

/*****************************************************************************/
//  Description : update album anim ctrl info
//  Global resource dependence :
//  Author: paul.huang
//  Note: 切换专辑图片信息
//        无专辑图片信息时默认专辑图片，非专辑显示模式时使用默认背景图片
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SwitchAnimCtrlInfo(
    BOOLEAN is_update,
    BOOLEAN is_prev
)
{
    MMI_CTRL_ID_T   mid_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
	if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID)) //Bug 132791 
	{
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
		//get pre/cur/next album info
		MMIAP_UpdateAllAlbumInfo(is_prev);

		if(MMIMP3_DISPLAY_ALBUM == APSETTING_GetDisplayStyle())
		{
			//set album info
			MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_LEFT, is_update);
			MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_MID, is_update);
			MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_RIGHT, is_update);
		}
		else
#endif
		{
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
			MMIAP_ReleaseAllAlbumImgInfo();
#endif

			//非专辑显示模式，使用默认背景
			SetAnimDefaultParam(mid_ctrl_id, is_update);

			//set ownerdraw callback
			GUIANIM_SetOwnerDraw(mid_ctrl_id, MainAnimOwnerDrawCallback);
		}

		//update artist and album name title display
		MMIAP_DisplayAlbumTitleInfo(is_update);

		//update up osd file tip
		MMIAP_UpdateOsdFileTip(is_update);
    
		GUIFORM_SetVisible(MMIMP3_OSD_PANEL_FORM_CTRL_ID, TRUE, TRUE);
		MMIMP3_DisplayButton(TRUE, is_update);
		MMIMP3_DisplayProgressAndTimeInfo(TRUE);
	}

}

/*****************************************************************************/
//  Description : init album anim ctrl param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitAnimCtrlParam(void)
{
    //init anim ctrl id
    MMIMP3_InitAlbumGlideCtrlId(TRUE);

    //set anim rect
    MMIAP_SetLeftAnimRect(FALSE);
    MMIAP_SetMidAnimRect(FALSE);
    MMIAP_SetRightAnimRect(FALSE);
}

/*****************************************************************************/
//  Description : modify glide anim control id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ModifyAnimGlideCtrlId(
    BOOLEAN    is_prev,
    BOOLEAN    is_next
)
{
    uint32          i = 0;
    uint32          max_index = MMIAP_DIRECT_MAX - 1;
    MMI_CTRL_ID_T   ctrl_id = 0;



    //将ctrl id循环赋值
    if(is_prev)
    {
        ctrl_id = MMIAP_GetAnimCtrlID(max_index);

        for(i = max_index; i > 0; i--)
        {
            SetAPAnimCtrlID(i, MMIAP_GetAnimCtrlID(i - 1));
        }

        SetAPAnimCtrlID(i, ctrl_id);
    }
    else if(is_next)
    {
        ctrl_id = MMIAP_GetAnimCtrlID(0);

        for(i = 0; i < max_index; i++)
        {
            SetAPAnimCtrlID(i, MMIAP_GetAnimCtrlID(i + 1));
        }

        SetAPAnimCtrlID(i, ctrl_id);
    }
    else
    {
        //nothing to do
    }
}

/*****************************************************************************/
//  Description : init album glide ctrl id
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_InitAlbumGlideCtrlId(BOOLEAN is_bg_player)
{

    SetAPAnimCtrlID(MMIAP_DIRECT_LEFT, MMIMP3_PRE_ALBUM_ANIM_CTRL_ID);
    SetAPAnimCtrlID(MMIAP_DIRECT_MID, MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID);
    SetAPAnimCtrlID(MMIAP_DIRECT_RIGHT, MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayLeftKeyButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_update
)
{
    GUI_BG_T btn_bg = {0};

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = IMAGE_COMMON_BTN_OPTION;//IMAGE_COMMON_BTN_LEFTKEY;

    if(is_bg_player)
    {
#ifndef TOUCH_PANEL_SUPPORT

        if(MMIAPIMP3_IsPlayerActive()
                && MMIAP_IsReviewOpen()
          )
        {
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

            switch(MMIMP3_GetReviewStatus())
            {
            case MMIMP3_REVIEW_STATE_NONE:
                btn_bg.img_id  = IMAGE_MP3_BTN_REVIEW_START;
                break;

            case MMIMP3_REVIEW_STATE_SET:
                btn_bg.img_id  = IMAGE_MP3_BTN_REVIEW_STOP;
                break;

            case MMIMP3_REVIEW_STATE_PLAY:
                btn_bg.img_id  = IMAGE_COMMON_BTN_OPTION;
                break;

            default:
                break;
            }

#endif
        }
        else
        {
            btn_bg.img_id  = IMAGE_COMMON_BTN_OPTION;
        }

#endif
        //GUIBUTTON_SetBg(MMIMP3_OPT_BTN_CTRL_ID, &btn_bg);
        GUIBUTTON_SetPressedFg(MMIMP3_OPT_BTN_CTRL_ID, &btn_bg);
        GUIBUTTON_SetFg(MMIMP3_OPT_BTN_CTRL_ID, &btn_bg);

        if(is_update)
        {
            GUIBUTTON_Update(MMIMP3_OPT_BTN_CTRL_ID);
        }
    }

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayReturnKeyButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_update
)
{
    GUI_BG_T btn_bg = {0};

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = IMAGE_COMMON_BTN_RETURN;

    if(is_bg_player)
    {
#ifndef TOUCH_PANEL_SUPPORT

        if(MMIAPIMP3_IsPlayerActive()
                && MMIAP_IsReviewOpen()
          )
        {
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

            switch(MMIMP3_GetReviewStatus())
            {
            case MMIMP3_REVIEW_STATE_NONE:
                btn_bg.img_id  = IMAGE_MP3_BTN_REVIEW_CANCEL;
                break;

            case MMIMP3_REVIEW_STATE_SET:
                btn_bg.img_id  = IMAGE_MP3_BTN_REVIEW_CANCEL;
                break;

            case MMIMP3_REVIEW_STATE_PLAY:
                btn_bg.img_id  = IMAGE_COMMON_BTN_RETURN;
                break;

            default:
                break;
            }

#endif
        }
        else
        {
            btn_bg.img_id  = IMAGE_COMMON_BTN_RETURN;
        }

#endif

        //GUIBUTTON_SetBg(MMIMP3_RETRUN_BTN_CTRL_ID, &btn_bg);
        GUIBUTTON_SetPressedFg(MMIMP3_RETRUN_BTN_CTRL_ID, &btn_bg);
        GUIBUTTON_SetFg(MMIMP3_RETRUN_BTN_CTRL_ID, &btn_bg);

        if(is_update)
        {
            GUIBUTTON_Update(MMIMP3_RETRUN_BTN_CTRL_ID);
        }
    }


}

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author: xingdong.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayVolumeButton(
    BOOLEAN is_bg_player,
    BOOLEAN is_update
)
{
    GUI_BG_T        btn_bg = {0};
    MMI_IMAGE_ID_T  image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T  press_image_id = IMAGE_NULL;
    MMI_CTRL_ID_T   ctrl_id  = MMIMP3_VOLUME_BTN_CTRL_ID;
    uint8           volume   = MMIAPISET_GetMultimVolume();

    if(0 == volume)
    {
#ifndef MMI_RES_LOW_COST
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL;
#endif
    }

    if(0 == volume)
    {
#ifndef MMI_RES_LOW_COST
        press_image_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_FOCUS;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST
        press_image_id = IMAGE_COMMON_PLAYER_BTN_VOL_FOCUS;
#endif
    }

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = press_image_id;
    GUIBUTTON_SetPressedFg(ctrl_id, &btn_bg);

    btn_bg.img_id  = image_id;
    GUIBUTTON_SetFg(ctrl_id, &btn_bg);

    if(is_update)
    {
        GUIBUTTON_Update(ctrl_id);
    }
}

/*****************************************************************************/
//  Description : Display mp3 play & pause button image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayPlayOrPauseButton(
    BOOLEAN           is_bg_player,
    BOOLEAN           is_focus_btn,
    BOOLEAN           is_update,
    MMI_WIN_ID_T      win_id
)
{
    MMI_IMAGE_ID_T          play_pause_image = IMAGE_COMMON_BTN_PLAY_GREY;
    MMI_IMAGE_ID_T          play_pause_press_img = IMAGE_NULL;
    MMIMP3_AUDIO_STATE_E    audio_state      = MMIMP3_GetAudioState();
    MMIMP3_PLAYER_STATE_E   player_state     = MMIAPIMP3_GetPlayerState();
    MMI_CTRL_ID_T           ctrl_id          = MMIMP3_PLAY_BTN_CTRL_ID;
    GUI_BG_T btn_bg = {0};
    GUI_BG_T press_bg = {0};

    if(MMIMP3_IsButtonValid(MMIMP3_BUTTON_PLAY_PAUSE))
    {
        switch(player_state)
        {
        case MMIMP3_PLAYER_STATE_NONE:
        case MMIMP3_PLAYER_SWITCH_PAUSED:
            play_pause_image = IMAGE_COMMON_BTN_PLAY;
#ifndef MMI_RES_LOW_COST
            play_pause_press_img = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif
            break;

        case MMIMP3_PLAYER_MUSIC_SWITCH:
            play_pause_image = IMAGE_COMMON_BTN_PAUSE;
#ifndef MMI_RES_LOW_COST
            play_pause_press_img = IMAGE_COMMON_BTN_PAUSE_FOCUS;
#endif
            break;

        case MMIMP3_PLAYER_AUDIO_ACTIVE:
            switch(audio_state)
            {
            case MMIMP3_AUDIO_PLAYING:
                play_pause_image = IMAGE_COMMON_BTN_PAUSE;
#ifndef MMI_RES_LOW_COST
                play_pause_press_img = IMAGE_COMMON_BTN_PAUSE_FOCUS;
#endif
                break;

            case MMIMP3_AUDIO_PAUSED:
            case MMIMP3_AUDIO_STOPPED:
            default:
                play_pause_image = IMAGE_COMMON_BTN_PLAY;
#ifndef MMI_RES_LOW_COST
                play_pause_press_img = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif
                break;
            }

            break;

        default:
            break;
        }
    }
    else
    {
        play_pause_image = IMAGE_COMMON_BTN_PLAY_GREY;
#ifndef MMI_RES_LOW_COST
        play_pause_press_img = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif
    }

    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = play_pause_image;
    press_bg.bg_type = GUI_BG_IMG;
    press_bg.img_id  = play_pause_press_img;
    GUIBUTTON_SetPressedFg(ctrl_id, &press_bg);
    GUIBUTTON_SetFg(ctrl_id, &btn_bg);

    if(is_update)
    {
        GUIBUTTON_Update(ctrl_id);
    }
}

/*****************************************************************************/
//  Description : Display eq or list button image.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayEqOrListButton(
    BOOLEAN is_update
)
{
#ifdef TOUCH_PANEL_SUPPORT

#ifdef MP3_EQ_SUPPORT
    MMI_IMAGE_ID_T      eq_or_list_img  = MMIMP3_IsButtonValid(MMIMP3_BUTTON_EQ_OR_LIST) ? IMAGE_COMMON_BTN_EQ : IMAGE_COMMON_BTN_EQ_GREY;
#else
    MMI_IMAGE_ID_T      eq_or_list_img  = MMIMP3_IsButtonValid(MMIMP3_BUTTON_EQ_OR_LIST) ? IMAGE_COMMON_BTN_LIST : IMAGE_COMMON_BTN_LIST_GREY;
#endif

    GUI_BG_T btn_bg = {0};



    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = eq_or_list_img;

    GUIBUTTON_SetPressedFg(MMIMP3_EQ_BTN_CTRL_ID, &btn_bg);
    GUIBUTTON_SetFg(MMIMP3_EQ_BTN_CTRL_ID, &btn_bg);

    if(is_update)
    {
        GUIBUTTON_Update(MMIMP3_EQ_BTN_CTRL_ID);
    }
#endif

}

/*****************************************************************************/
//  Description : Display preq button image.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayPreButton(
    BOOLEAN is_update
)
{
    MMI_IMAGE_ID_T pre_image = MMIMP3_IsButtonValid(MMIMP3_BUTTON_PREV) ? IMAGE_COMMON_BTN_PREV : IMAGE_COMMON_BTN_PREV_GREY;

    GUI_BG_T btn_bg = {0};
    GUI_BG_T pre_bg = {0};



    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = pre_image;

    pre_bg.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
    pre_bg.img_id  = IMAGE_COMMON_BTN_PREV_FOCUS;
#endif

    GUIBUTTON_SetPressedFg(MMIMP3_PREV_BTN_CTRL_ID, &pre_bg);
    GUIBUTTON_SetFg(MMIMP3_PREV_BTN_CTRL_ID, &btn_bg);

    if(is_update)
    {
        GUIBUTTON_Update(MMIMP3_PREV_BTN_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : Display preq button image.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayNextButton(
    BOOLEAN is_update
)
{
    MMI_IMAGE_ID_T next_image = MMIMP3_IsButtonValid(MMIMP3_BUTTON_NEXT) ? IMAGE_COMMON_BTN_NEXT : IMAGE_COMMON_BTN_NEXT_GREY;

    GUI_BG_T btn_bg = {0};
    GUI_BG_T pre_bg = {0};


    btn_bg.bg_type = GUI_BG_IMG;
    btn_bg.img_id  = next_image;

    pre_bg.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
    pre_bg.img_id  = IMAGE_COMMON_BTN_NEXT_FOCUS;
#endif
    GUIBUTTON_SetPressedFg(MMIMP3_NEXT_BTN_CTRL_ID, &pre_bg);
    GUIBUTTON_SetFg(MMIMP3_NEXT_BTN_CTRL_ID, &btn_bg);

    if(is_update)
    {
        GUIBUTTON_Update(MMIMP3_NEXT_BTN_CTRL_ID);
    }
}
/*****************************************************************************/
//  Description : display mp3 play window button image
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayButton(
    BOOLEAN is_bg_player,
    BOOLEAN is_update
)
{
#ifdef MAINLCD_SIZE_128X64
#else

    MMI_WIN_ID_T        win_id          = MMIMP3_MAIN_PLAY_WIN_ID;



    MMIMP3_DisplayShuffleButton(is_bg_player, FALSE, is_update, win_id);
    MMIMP3_DisplayRepeatButton(is_bg_player, FALSE, is_update, win_id);
    MMIMP3_DisplayPlayOrPauseButton(is_bg_player, FALSE, is_update, win_id);


    DisplayEqOrListButton(is_update);
    DisplayPreButton(is_update);
    DisplayNextButton(is_update);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

    MMIMP3_DisplayReviewButton(is_bg_player, FALSE, is_update, win_id);
#endif
    MMIMP3_DisplayLeftKeyButton(is_bg_player, is_update);
    MMIMP3_DisplayReturnKeyButton(is_bg_player, is_update);
    MMIMP3_DisplayVolumeButton(is_bg_player, is_update);

    /*
    if(MMK_IsFocusWin(win_id))
    {
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
    }
    */
#endif

#ifdef BT_AV_CT_SUPPORT
	if(MMIAPIMp3_IsBtMusic())
	{
		UpdateAvrcpPlayerButton();
	}
#endif
}

/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgress(
    BOOLEAN is_bg_player
)
{
    uint16              slide_display_x_max = 0;
    uint16              cur_slide_point = 0;
    uint16              slide_review_start = 0;
    uint16              slide_review_end = 0;
    uint32              review_start_offset = 0;
    uint32              review_end_offset = 0;
    GUI_RECT_T          progress_image_rect = {0};
    GUI_RECT_T          progress_lcd_rect = {0};
    GUI_RECT_T          progress_groove_rect = {0};
    GUI_RECT_T          groove_bg_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T        win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID;
    MMIMP3_PLAYER_STATE_E   player_state     = MMIAPIMP3_GetPlayerState();
    MMIMP3_REVIEW_STATE_TYPE_E review_state = MMIMP3_REVIEW_STATE_NONE;
    BOOLEAN             is_seek_support = FALSE;
    GUI_POINT_T         dis_point = {0};
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    uint16              progress_width = 0;
    uint16              progress_height = 0;
    uint16              slide_width = 0;
    uint16              slide_height = 0;
    int16               slide_x_offset = 0;
    int16               slide_y_offset = 0;
    uint16              review_slide_width = 0;
    uint16              review_slide_height = 0;
    int16               review_slide_x_offset = 0;
    int16               review_slide_y_offset = 0;
    uint16              review_slide_display_x_max = 0;
    uint16              cur_review_slide_point = 0;
#ifdef BT_AV_CT_SUPPORT				
	AvrcpCtPlayStatus cur_avrcp_play_status = MMIAPIMp3_GetAvrcpPlayStatus();
#endif
    
    MMIMP3_GetProgressGrooveRect(is_bg_player, &progress_groove_rect);
    MMIMP3_GetProgressRect(is_bg_player, &progress_lcd_rect);
    MMIMP3_GetProgressSlideWidthHeight(&slide_width, &slide_height, win_id);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    MMIMP3_GetReviewSlideWidthHeight(&review_slide_width, &review_slide_height, win_id);
#endif
    progress_width = progress_lcd_rect.right - progress_lcd_rect.left + 1;
    progress_height = progress_lcd_rect.bottom - progress_lcd_rect.top + 1;
    progress_image_rect.right = progress_width - 1;
    progress_image_rect.bottom = progress_height - 1;

    slide_x_offset = - slide_width / 2;
    slide_y_offset = (progress_height - slide_height) / 2; // - 1;

    review_slide_x_offset = - review_slide_width / 2;
    review_slide_y_offset = (progress_height - review_slide_height) / 2; // - 1;

    if(is_bg_player)
    {
        //if(MMIAPIMP3_GetPlayerState())
        MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);
        is_seek_support = MMIMP3_IsCurFileSeekSupport();
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
        review_state = MMIMP3_GetReviewStatus();
#endif
    }
    else
    {
        #ifdef  MUSIC_PLAYER_SUPPORT
	    if(PNULL != dyn_info)
	    {
	        MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);
	    }
        is_seek_support = MMIMP3_IsCurFileSeekSupport();
        review_state = MMIMP3_REVIEW_STATE_NONE;
        #endif
    }

    //if (//MMK_IsFocusWin(win_id) &&
    //is_seek_support)
    {
        GUIOWNDRAW_GetDisplayRect(ctrl_id, &groove_bg_rect);

        //display progress groove
        //dis_point.x = progress_groove_rect.left;
        //dis_point.y = progress_groove_rect.top;
        GUIRES_DisplayImg(
            PNULL,//&dis_point,
            &progress_groove_rect,//PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_PLAYER_PROCESS_GROOVE,
            &lcd_dev_info);

        if((MMIMP3_PLAYER_AUDIO_ACTIVE == player_state)
                || MMIAPIAP_IsDisplayForPlayStateNone(TRUE, FALSE)
#ifdef BT_AV_CT_SUPPORT
				|| MMIAPIMp3_IsBtMusic()
#endif
          )
        {
            if( //(0 != dyn_info.cur_offset) &&
                (0 != dyn_info.total_length)
#ifdef BT_AV_CT_SUPPORT
				|| MMIAPIMp3_IsBtMusic()
#endif
                )
            {
                slide_display_x_max = progress_groove_rect.left + progress_width - slide_width/*/2*/ + 1;
                review_slide_display_x_max = progress_groove_rect.left + progress_width - review_slide_width/*/2*/ + 1;

#ifdef BT_AV_CT_SUPPORT
				if(MMIAPIMp3_IsBtMusic())
				{
                    if(cur_avrcp_play_status.Length > 0)
                    {
					    cur_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)cur_avrcp_play_status.Position/cur_avrcp_play_status.Length);	
					    cur_review_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)cur_avrcp_play_status.Position/cur_avrcp_play_status.Length);
                    }
                    else
                    {
                        cur_slide_point = 0;	
					    cur_review_slide_point = 0;
                    }
				}
				else
				{
					cur_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)dyn_info.cur_offset/dyn_info.total_length);
					cur_review_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)dyn_info.cur_offset/dyn_info.total_length);
				}
#else
				cur_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)dyn_info.cur_offset/dyn_info.total_length);
                cur_review_slide_point = (uint16)((progress_width - slide_width/*/2*/ + 1)*(uint64_t)dyn_info.cur_offset/dyn_info.total_length);
#endif

                switch(review_state)
                {
                case MMIMP3_REVIEW_STATE_NONE:
                    progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(cur_slide_point + slide_width / 2, progress_width));

                    if(progress_lcd_rect.right > progress_lcd_rect.left)
                    {
                        GUIRES_DisplayImg(PNULL,
                                          &progress_lcd_rect,
                                          PNULL,//&progress_image_rect,
                                          win_id,
                                          IMAGE_COMMON_PLAYER_PROCESS,
                                          &lcd_dev_info);
                    }

                    IMG_EnableTransparentColor(TRUE);
                    dis_point.x = /*slide_x_offset + */MMI_MIN(progress_groove_rect.left + cur_slide_point, slide_display_x_max);
                    dis_point.y = slide_y_offset + progress_groove_rect.top;
                    GUIRES_DisplayImg(&dis_point,
                                      PNULL,
                                      PNULL,
                                      win_id,
                                      IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                                      &lcd_dev_info);
                    IMG_EnableTransparentColor(FALSE);
                    break;
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

                case MMIMP3_REVIEW_STATE_SET:
                    if(MMIAPIMP3_GetReviewStartPoint(&review_start_offset))
                    {
                        slide_review_start = (uint16)((progress_width - slide_width/*/2*/ + 1) * (uint64_t)review_start_offset / dyn_info.total_length);

                        //draw the background section
                        progress_lcd_rect.right = (uint16)(progress_lcd_rect.left + (int16)(MMI_MIN(slide_review_start + slide_width / 2, progress_width)));

                        if(progress_lcd_rect.right > progress_lcd_rect.left)
                        {
                            GUIRES_DisplayImg(PNULL,
                                              &progress_lcd_rect,
                                              PNULL,//&progress_image_rect,
                                              win_id,
                                              IMAGE_COMMON_PLAYER_PROCESS,
                                              &lcd_dev_info);
                        }

                        //draw the current playing section
                        progress_lcd_rect.left += (int16)(MMI_MIN(slide_review_start, progress_width));
                        progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(cur_slide_point - slide_review_start + slide_width / 2, progress_width));

                        if(cur_slide_point > slide_review_start
                                && progress_lcd_rect.right > progress_lcd_rect.left)
                        {
                            GUIRES_DisplayImg(PNULL,
                                              &progress_lcd_rect,
                                              PNULL,//&progress_image_rect,
                                              win_id,
                                              IMAGE_MP3_PROCESS_REVIEW,
                                              &lcd_dev_info);
                        }

                        //draw slides
                        IMG_EnableTransparentColor(TRUE);
                        dis_point.x = /*review_slide_x_offset + */MMI_MIN(progress_groove_rect.left + slide_review_start, slide_display_x_max);
                        dis_point.y = review_slide_y_offset + progress_groove_rect.top;

                        GUIRES_DisplayImg(&dis_point,
                                          PNULL,
                                          PNULL,
                                          win_id,
                                          IMAGE_MP3_PROCESS_SLIDE_REVIEW,
                                          &lcd_dev_info);

                        dis_point.x = /*slide_x_offset + */MMI_MIN(progress_groove_rect.left + cur_slide_point, slide_display_x_max);
                        dis_point.y = slide_y_offset + progress_groove_rect.top;
                        GUIRES_DisplayImg(&dis_point,
                                          PNULL,
                                          PNULL,
                                          win_id,
                                          IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                                          &lcd_dev_info);
                        IMG_EnableTransparentColor(FALSE);
                    }

                    break;

                case MMIMP3_REVIEW_STATE_PLAY:
                    if(MMIAPIMP3_GetReviewStartPoint(&review_start_offset)  &&
                            MMIAPIMP3_GetReviewEndPoint(&review_end_offset))
                    {
                        slide_review_start = (uint16)((progress_width - slide_width/*/2*/ + 1) * (uint64_t)review_start_offset / dyn_info.total_length);
                        slide_review_end   = (uint16)((progress_width - slide_width/*/2*/ + 1) * (uint64_t)review_end_offset / dyn_info.total_length);

                        if(cur_slide_point > slide_review_end)
                        {
                            cur_slide_point = slide_review_end;
                        }

                        if(cur_slide_point < slide_review_start)
                        {
                            cur_slide_point = slide_review_start;
                        }

                        //draw the selected section
                        progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(slide_review_end + slide_width / 2, progress_width - 1));

                        if(progress_lcd_rect.right > progress_lcd_rect.left)
                        {
                            GUIRES_DisplayImg(PNULL,
                                              &progress_lcd_rect,
                                              PNULL,//&progress_image_rect,
                                              win_id,
                                              IMAGE_COMMON_PLAYER_PROCESS,
                                              &lcd_dev_info);
                        }

                        //draw the current playing section
                        progress_lcd_rect.left += (int16)(MMI_MIN(slide_review_start, progress_width));
                        progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(cur_slide_point - slide_review_start + slide_width / 2, progress_width));

                        if(cur_slide_point > slide_review_start
                                && progress_lcd_rect.right > progress_lcd_rect.left)
                        {
                            GUIRES_DisplayImg(PNULL,
                                              &progress_lcd_rect,
                                              PNULL,//&progress_image_rect,
                                              win_id,
                                              IMAGE_MP3_PROCESS_REVIEW,
                                              &lcd_dev_info);
                        }

                        //draw slides
                        IMG_EnableTransparentColor(TRUE);
                        dis_point.x = /*review_slide_x_offset + */MMI_MIN(progress_groove_rect.left + slide_review_start, slide_display_x_max);
                        dis_point.y = review_slide_y_offset + progress_groove_rect.top;

                        GUIRES_DisplayImg(&dis_point,
                                          PNULL,
                                          PNULL,
                                          win_id,
                                          IMAGE_MP3_PROCESS_SLIDE_REVIEW,
                                          &lcd_dev_info);

                        dis_point.x = /*review_slide_x_offset + */MMI_MIN(progress_groove_rect.left + slide_review_end, slide_display_x_max);
                        dis_point.y = review_slide_y_offset + progress_groove_rect.top;
                        GUIRES_DisplayImg(&dis_point,
                                          PNULL,
                                          PNULL,
                                          win_id,
                                          IMAGE_MP3_PROCESS_SLIDE_REVIEW,
                                          &lcd_dev_info);

                        dis_point.x = /*slide_x_offset + */MMI_MIN(progress_groove_rect.left + cur_slide_point, slide_display_x_max);
                        dis_point.y = slide_y_offset + progress_groove_rect.top;
                        GUIRES_DisplayImg(&dis_point,
                                          PNULL,
                                          PNULL,
                                          win_id,
                                          IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                                          &lcd_dev_info);
                        IMG_EnableTransparentColor(FALSE);
                    }

                    break;
#endif

                default:
                    break;

                }
            }
            else
            {
                IMG_EnableTransparentColor(TRUE);
                dis_point.x = /*slide_x_offset + */progress_groove_rect.left;
                dis_point.y = slide_y_offset + progress_groove_rect.top;
                GUIRES_DisplayImg(&dis_point,
                                  PNULL,
                                  PNULL,
                                  win_id,
                                  IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                                  &lcd_dev_info);
                IMG_EnableTransparentColor(FALSE);
            }

            //SCI_TRACE_LOW:"MMIMP3_DisplayProgress:uiCurOffset = %d,uiTotalLength = %d,uiCurTime = %d,uiTotalTime = %d,cur_slide = %d,review_start = %d, review_end = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_5924_112_2_18_2_4_49_310, (uint8*)"ddddddd", dyn_info.cur_offset, dyn_info.total_length, dyn_info.cur_time, dyn_info.total_time, cur_slide_point, slide_review_start, slide_review_end);
        }

        //MMIMP3_DisplayTimeInfo(is_bg_player);//暂留
    }
}

/*****************************************************************************/
//  Description : Display the end of the progress.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgressEnd(
    BOOLEAN is_bg_player
)
{
    GUI_POINT_T         dis_point = {0};
    uint16              slide_display_x = 0;
    GUI_RECT_T          progress_image_rect = {0};
    GUI_RECT_T          progress_lcd_rect   = {0};
    GUI_RECT_T          progress_groove_rect = {0};
    GUI_RECT_T          groove_bg_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T        win_id =  MMIMP3_MAIN_PLAY_WIN_ID ;
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID;
    BOOLEAN             is_seek_support = FALSE;

    uint16              progress_width = 0;
    uint16              progress_height = 0;
    uint16              slide_width = 0;
    uint16              slide_height = 0;
    int16               slide_x_offset = 0;
    int16               slide_y_offset = 0;

    MMIMP3_GetProgressGrooveRect(is_bg_player, &progress_groove_rect);
    MMIMP3_GetProgressRect(is_bg_player, &progress_lcd_rect);
    MMIMP3_GetProgressSlideWidthHeight(&slide_width, &slide_height, win_id);

    progress_width = progress_lcd_rect.right - progress_lcd_rect.left + 1;
    progress_height = progress_lcd_rect.bottom - progress_lcd_rect.top + 1;
    progress_image_rect.right = progress_width - 1;
    progress_image_rect.bottom = progress_height - 1;

    slide_x_offset = - slide_width / 2;
    slide_y_offset = (progress_height - slide_height) / 2; // - 1;

    if(is_bg_player)
    {
        is_seek_support = MMIMP3_IsCurFileSeekSupport();
    }

    //if (//MMK_IsFocusWin(win_id)   &&
    //is_seek_support)
    {
        GUIOWNDRAW_GetDisplayRect(ctrl_id, &groove_bg_rect);

        slide_display_x = progress_groove_rect.left + progress_width - slide_width/*/2*/ + 1;
        progress_lcd_rect.right = progress_lcd_rect.left + progress_width;

        //dis_point.x = progress_groove_rect.left;
        //dis_point.y = progress_groove_rect.top;

        GUIRES_DisplayImg(
            PNULL,//&dis_point,
            &progress_groove_rect,//PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_PLAYER_PROCESS_GROOVE,
            &lcd_dev_info
        );

        GUIRES_DisplayImg(
            PNULL,
            &progress_lcd_rect,
            PNULL,//&progress_image_rect,
            win_id,
            IMAGE_COMMON_PLAYER_PROCESS,
            &lcd_dev_info
        );

        IMG_EnableTransparentColor(TRUE);
        dis_point.x = /*slide_x_offset+*/slide_display_x;
        dis_point.y = slide_y_offset + progress_groove_rect.top;
        GUIRES_DisplayImg(
            &dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
            &lcd_dev_info
        );
        IMG_EnableTransparentColor(FALSE);

        //MMIMP3_DisplayTimeInfo(is_bg_player);
    }
}

/*****************************************************************************/
//  Description : MMIMP3_GetTimeRect
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetTimeRect(BOOLEAN is_bg_player,
                               GUI_RECT_T *cur_rect_ptr,
                               GUI_RECT_T *total_rect_ptr
                              )
{
    GUI_RECT_T        rect = {0};
    static GUI_RECT_T s_cur_rect = {0};
    static GUI_RECT_T s_total_rect = {0};
    static BOOLEAN    s_is_horizon = FALSE;
    MMI_CTRL_ID_T     ctrl_id = MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID;

    if(PNULL == cur_rect_ptr || PNULL == total_rect_ptr)
    {
        return;
    }

    if(GUI_IsRectEmpty(s_cur_rect) || GUI_IsRectEmpty(s_total_rect))
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();
    }

    if(GUI_IsRectEmpty(s_cur_rect) || GUI_IsRectEmpty(s_total_rect) || s_is_horizon != MMITHEME_IsMainScreenLandscape())
    {
        s_is_horizon = MMITHEME_IsMainScreenLandscape();

        //MMIMP3_GetProgressRect(is_bg_player,&rect);
        GUIOWNDRAW_GetDisplayRect(ctrl_id, &rect);

        s_cur_rect.left   = rect.left + MMIMP3_TIME_X_OFFSET + MMIMP3_PROGRESS_HORIZ_MARGIN;
        s_cur_rect.top    = rect.top + MMIMP3_TIME_Y_OFFSET;
        s_cur_rect.right  = s_cur_rect.left + MMIMP3_TIME_WIDTH - 1;
        s_cur_rect.bottom = s_cur_rect.top + MMIMP3_TIME_HEIGHT;//- 1;

        s_total_rect.right  = rect.right - MMIMP3_TIME_X_OFFSET - MMIMP3_PROGRESS_HORIZ_MARGIN;
        s_total_rect.top    = rect.top + MMIMP3_TIME_Y_OFFSET;
        s_total_rect.left   = s_total_rect.right - MMIMP3_TIME_WIDTH + 1;
        s_total_rect.bottom = s_total_rect.top + MMIMP3_TIME_HEIGHT;//- 1;
    }

    *cur_rect_ptr   = s_cur_rect;
    *total_rect_ptr = s_total_rect;
}

/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayProgressAndTimeInfo(
    BOOLEAN is_bg_player
)
{
    MMI_CTRL_ID_T   ctrl_id1 =  MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID;
    MMI_CTRL_ID_T   ctrl_id2 =  MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID;

    if(!MMIAP_IsOpenOsdPanel())
    {
        return;
    }

    // #ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
    //  if(MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
    //  {
    //      return;
    //  }
    // #endif
    GUIOWNDRAW_Update(ctrl_id1);
    GUIOWNDRAW_Update(ctrl_id2);
}

/*****************************************************************************/
//  Description : Display current mp3 playing time.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayTimeInfo(
    BOOLEAN is_bg_player
)
{
    uint8           cur_time_str[32]    = {0};
    uint8           total_time_str[32]  = {0};
    wchar           cur_time_wstr[32]   = {0};
    wchar           total_time_wstr[32] = {0};

    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};

    BOOLEAN         is_seek_support    = FALSE;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   cur_time_rect          = {0};  /*lint !e64*/
    GUI_RECT_T   total_time_rect        = {0};  /*lint !e64*/
    GUI_RECT_T   time_bg_rect           = {0};  /*lint !e64*/
    GUISTR_STYLE_T      text_style      = {0};  /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_SINGLE_LINE;
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID ;
#ifdef BT_AV_CT_SUPPORT				
	AvrcpCtPlayStatus cur_avrcp_play_status = MMIAPIMp3_GetAvrcpPlayStatus();
#endif
    
    MMIMP3_GetTimeRect(is_bg_player, &cur_time_rect, &total_time_rect);

    if(is_bg_player)
    {
        MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);
        is_seek_support = MMIMP3_IsCurFileSeekSupport();
    }

    //if(MMK_IsFocusWin(win_id))
    {
        //if(is_seek_support)
        {
            if(dyn_info.total_time / 60 < 60)
            {
                cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", dyn_info.cur_time / 60, dyn_info.cur_time % 60);
                total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", dyn_info.total_time / 60, dyn_info.total_time % 60);
            }
            else
            {
                cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.cur_time / 60) / 60,
                                             (dyn_info.cur_time / 60) % 60,
                                             (dyn_info.cur_time % 60));
                total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.total_time / 60) / 60,
                                               (dyn_info.total_time / 60) % 60,
                                               dyn_info.total_time % 60);
            }

#ifdef BT_AV_CT_SUPPORT
			if(MMIAPIMp3_IsBtMusic())
			{
                if(cur_avrcp_play_status.Length > 0)
                {
				    cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", cur_avrcp_play_status.Position/60, cur_avrcp_play_status.Position%60);
				    total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", cur_avrcp_play_status.Length/60, cur_avrcp_play_status.Length%60);
                }
                else
                {
                    cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", 0, 0);
				    total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", 0, 0);
                }
			}
#endif

            cur_str_t.wstr_ptr = cur_time_wstr;
            total_str_t.wstr_ptr = total_time_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8*)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);

            text_style.align = ALIGN_LEFT;
            text_style.font = MMIMP3_TIME_FONT;
            text_style.font_color = MMIMP3_TIME_COLOR;

            GUIOWNDRAW_GetDisplayRect(ctrl_id, &time_bg_rect);

            // display
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_time_rect,       //the fixed display area
                (const GUI_RECT_T      *)&cur_time_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
            );

            // display
            text_style.align = ALIGN_RIGHT;
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&total_time_rect,       //the fixed display area
                (const GUI_RECT_T      *)&total_time_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&total_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
            );
        }
    }
}

/*****************************************************************************/
//  Description : display current mp3 name
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayFileName(
    BOOLEAN  is_update      //is update
)
{
    MMIMP3_LIST_FILE_BODY_T     cur_play_info = {0};
    wchar                       file_name[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16                      file_name_len = 0;
    //MMI_WIN_ID_T                win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMI_STRING_T                title_string = {0};

	SCI_TRACE_LOW("[MMIMP3]:MMIMP3_DisplayFileName");

    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name,&(cur_play_info.full_path_len)))
    {
        MMIAPIFMM_SplitFullPath(
            cur_play_info.full_path_name,
            cur_play_info.full_path_len,
            PNULL, PNULL, PNULL, PNULL,
            file_name, &file_name_len);


        title_string.wstr_ptr = file_name;
        title_string.wstr_len = file_name_len;

#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        GUITITLE_SetText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), title_string.wstr_ptr, title_string.wstr_len, is_update);
#else
        GUILABEL_SetText(MMIMP3_TITLE_LABLE_CTRL_ID, &title_string, is_update);
#endif
        //GUIWIN_SetTitleText(win_id, file_name, file_name_len, is_update);
    }
    else
    {
        //title_string.wstr_ptr = file_name;
        //title_string.wstr_len = file_name_len;
        //MMI_GetLabelTextByLang( TXT_AUDIO, &title_string );
        //GUILABEL_SetText(MMIMP3_TITLE_LABLE_CTRL_ID, &title_string, is_update);
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        GUITITLE_SetTextId(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), TXT_AUDIO, is_update);
#else
        GUILABEL_SetTextById(MMIMP3_TITLE_LABLE_CTRL_ID, TXT_AUDIO, is_update);
#endif
        //GUIWIN_SetTitleText(win_id, file_name, file_name_len, is_update);
    }
}

/*****************************************************************************/
//  Description : Init mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_InitTitleAnim(
    BOOLEAN is_bg_player
)
{
#ifdef MMIMP3_TITLE_ANIM_ENABLE
    MMI_CTRL_ID_T   anim_ctrl_id = MMIMP3_LITTLE_PLAY_ANIM_CTRL_ID;
    MMI_ANIM_ID_T   anim_id = MP3_IMAGE_LITTLE_PLAY_ANIM;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info    = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = anim_ctrl_id;
    control_info.ctrl_ptr = PNULL;

    data_info.anim_id = anim_id;
    data_info.type = GUIANIM_TYPE_ANIM;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update = FALSE;

    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : Clear mp3 anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopMainAnim(
    BOOLEAN is_bg_player
)
{
    //#ifdef MMIMP3_MIAN_PLAY_ANIM_ENABLE
    MMI_CTRL_ID_T   anim_ctrl_id = MMIMP3_MIAN_PLAY_ANIM_CTRL_ID;
    MMI_WIN_ID_T    win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMIMP3_AUDIO_STATE_E play_state = MMIMP3_AUDIO_STOPPED;

    if(is_bg_player)
    {
        play_state = MMIMP3_GetAudioState();
    }

    if(MMIMP3_AUDIO_PLAYING != play_state &&
            MMK_IsOpenWin(win_id))
    {
        if(PNULL != MMK_GetCtrlPtr(anim_ctrl_id))
        {
            //GUIANIM_PauseAnim(anim_ctrl_id);
            MMK_DestroyDynaCtrl(anim_ctrl_id);
        }
    }

    //#endif
}

/*****************************************************************************/
//  Description : Pause mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StopTitleAnim(
    BOOLEAN is_bg_player
)
{
#ifdef MMIMP3_TITLE_ANIM_ENABLE
    MMI_CTRL_ID_T   anim_ctrl_id = MMIMP3_LITTLE_PLAY_ANIM_CTRL_ID;
    MMI_WIN_ID_T    win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMIMP3_AUDIO_STATE_E play_state = MMIMP3_AUDIO_STOPPED;

    if(is_bg_player)
    {
        play_state = MMIMP3_GetAudioState();
    }

    if(MMIMP3_AUDIO_PLAYING != play_state &&
            MMK_IsOpenWin(win_id))
    {
        GUIANIM_PauseAnim(anim_ctrl_id);
    }

#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : Resume mp3 title anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_PlayTitleAnim(
    BOOLEAN is_bg_player
)
{
#ifdef MMIMP3_TITLE_ANIM_ENABLE
    MMI_CTRL_ID_T   anim_ctrl_id = MMIMP3_LITTLE_PLAY_ANIM_CTRL_ID;
    MMI_WIN_ID_T    win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    MMIMP3_AUDIO_STATE_E play_state = MMIMP3_AUDIO_STOPPED;

    if(is_bg_player)
    {
        play_state = MMIMP3_GetAudioState();
    }

    if(MMIMP3_AUDIO_PLAYING == play_state &&
            MMK_IsFocusWin(win_id))
    {
        GUIANIM_ResumeAnim(anim_ctrl_id);
    }

#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : Clear mp3 anim.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearMainAnim(
    BOOLEAN is_bg_player
)
{
    //#ifdef MMIMP3_MIAN_PLAY_ANIM_ENABLE
    MMI_CTRL_ID_T   anim_ctrl_id = MMIMP3_MIAN_PLAY_ANIM_CTRL_ID ;

    if(PNULL != MMK_GetCtrlPtr(anim_ctrl_id))
    {
        MMK_DestroyDynaCtrl(anim_ctrl_id);
    }

    //#endif
}

/*****************************************************************************/
//  Description : set mp3 as call ring
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SetMp3AsCallRing(
    MN_DUAL_SYS_E dual_sys
)
//uint16        *music_full_path_name,
//uint16        music_full_path_len,
{

    MMIMP3_LIST_FILE_BODY_T selected_file = {0};

    MMI_WIN_ID_T win_id = MMIMP3_PLAY_LIST_WIN_ID;
    BOOLEAN is_music_list = FALSE;
    uint16 list_index = 0;
    uint16 file_index = 0;


    selected_file.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMK_IsOpenWin(win_id))
    {
        is_music_list = MMIAPMULTILIST_IsMusicList(win_id);
        list_index = MMIAPMULTILIST_GetListSelection(win_id);
        file_index = MMIAPMULTILIST_GetFileIndex(win_id);
        if(MMIMP3_GetAudioFileFullPath(is_music_list,
                                       list_index,
                                       file_index,
                                       selected_file.full_path_name,
                                       &(selected_file.full_path_len)
                                      )
          )
        {
            MMIAPIFMM_SetMusicToRing(
                MMIMULTIM_SET_CALL_RING, (DPARAM)&dual_sys,
                selected_file.full_path_name, selected_file.full_path_len
            );
        }
        else
        {
            MMIMP3_FatalError();
        }
    }
    else
    {
        if(MMIMP3_GetCurPlayFileFullPath(selected_file.full_path_name,&(selected_file.full_path_len)))
        {
            MMIAPIFMM_SetMusicToRing(
                MMIMULTIM_SET_CALL_RING, (DPARAM)&dual_sys,
                selected_file.full_path_name, selected_file.full_path_len
            );
        }
    }
}

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopOperaSDPlug(void)
{
    //SCI_TRACE_LOW:"MMIAPIMP3_StopOperaSDPlug"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_7974_112_2_18_2_4_53_319, (uint8*)"");


    MMIAPIMP3_StopAudioPlayer();

    MMIAPIMP3_StopApplet(FALSE);
}

/*****************************************************************************/
//  Description : stop current operation when bt receive file
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopOperaBtAction(void)
{
    //SCI_TRACE_LOW:"MMIAPIMP3_StopOperaBtAction"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_7994_112_2_18_2_4_53_320, (uint8*)"");

    MMIAPIMP3_StopApplet(FALSE);
}

/*****************************************************************************/
//  Description : set mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3AlertInfo(
    MMIMP3_ALERT_MSG_E      alert_type,
    MMIMP3_ALERT_CALLBACK   alert_callback,
    MMI_WIN_ID_T            win_id
)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();


    if (PNULL == info_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[MMIMP3] SetMp3AlertInfo type = %d, callback = 0x%x, win_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4492_112_2_18_2_4_46_304, (uint8*)"ddd", alert_type, alert_callback, win_id);
    info_ptr->alert_type     = alert_type;
    info_ptr->alert_callback = alert_callback;
    info_ptr->win_id = win_id;
}

/*****************************************************************************/
//  Description : get mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetMp3AlertInfo(
    MMIAP_ALERT_INFO_T *alert_ptr //out
)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();


    if (PNULL != info_ptr)
    {
        *alert_ptr = *info_ptr;
    }
}

/*****************************************************************************/
//  Description : set mp3 alert type.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3AlertType(MMIMP3_ALERT_MSG_E alert_type)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->alert_type = alert_type;
    }
}

/*****************************************************************************/
//  Description : get mp3 alert type.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_ALERT_MSG_E GetMp3AlertType(void)
{
    MMIAP_ALERT_INFO_T *info_ptr = GetAPAlertInfoPtr();
    MMIMP3_ALERT_MSG_E alert_type = MMIMP3_ALERT_MSG_MAX;


    if (PNULL != info_ptr)
    {
        alert_type = info_ptr->alert_type;
    }

    return alert_type;
}

/*****************************************************************************/
//  Description : start mp3 alert timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartMp3AlertTimer(
    void
)
{
    uint8 timer_id = 0;
    MMIAP_ALERT_INFO_T alert_info = {0};

    //SCI_TRACE_LOW:"[MMIMP3]: StartMp3AlertTimer enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4543_112_2_18_2_4_46_305, (uint8*)"");


    timer_id = GetAPAlertTimerID();
    if(0 == timer_id)
    {
        GetMp3AlertInfo(&alert_info);
        timer_id = MMK_CreateWinTimer(alert_info.win_id, MMIMP3_ALERT_DISPLAY_PERIOD, FALSE);
        SetAPAlertTimerID(timer_id);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: StartMp3AlertTimer: timer started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4550_112_2_18_2_4_46_306, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop mp3 alert timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopMp3AlertTimer(
    void
)
{
    uint8 timer_id = 0;

    //SCI_TRACE_LOW:"[MMIMP3]: StopMp3AlertTimer enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4562_112_2_18_2_4_46_307, (uint8*)"");

    timer_id = GetAPAlertTimerID();
    if(0 < timer_id)
    {
        MMK_StopTimer(timer_id);
        SetAPAlertTimerID(0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: StopMp3AlertTimer: timer stopped already!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4570_112_2_18_2_4_46_308, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : get mp3 alert text id.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetMp3AlertTextId(
    MMIMP3_ALERT_MSG_E  alert_type
)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    switch(alert_type)
    {
    case MMIMP3_MUSIC_FILE_NOT_EXIST:
        text_id = TXT_COM_FILE_NO_EXIST;//TXT_MP3_MUSIC_FILE_NOT_EXIST;
        break;

    case MMIMP3_MUSIC_FILE_SIZE_ZERO:
        text_id = TXT_EMPTY_FILE;
        break;

    case MMIMP3_A2DP_NOT_SUPPORT:
        text_id = TXT_MP3_NOT_SUPPORT_A2DP;
        break;

    case MMIMP3_FORMAT_NOT_SUPPORT:
        text_id = TXT_COMMON_NO_SUPPORT;
        break;
#ifdef  BLUETOOTH_SUPPORT

    case MMIMP3_BT_NOT_SUPPORT_EQ:
        text_id = TXT_COMMON_BT_NOT_SUPPORT_EQ;
        break;
#endif
#ifdef DRM_SUPPORT

    case MMIMP3_DRM_NOT_SYS_NITZ:
        text_id = TXT_DRM_NOT_SYS_NITZ;
        break;

    case MMIMP3_DRM_RIGHT_INVALID:
        text_id = TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE;
        break;

    case MMIMP3_DRM_FORMAT_NOT_AUDIO:
        text_id = TXT_COMMON_NO_SUPPORT;
        break;
#endif

    default:
        break;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : display mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplayAlertInfo(
    void
)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T  text_str = {0};
    uint8  line_space = 1;
    uint16 char_space = 1;

    GUISTR_STYLE_T  text_style  = {0};  /*lint !e64*/
    GUISTR_STATE_T  state       =  GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;

    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T       alert_rect = {0};

    //if (MMK_IsFocusWin(win_id))
    {
        text_id = GetMp3AlertTextId(GetMp3AlertType());

        if(TXT_NULL != text_id)
        {
            GUIOWNDRAW_GetDisplayRect(MMIMP3_MAIN_OWNER_DRAW_CTRL_ID, &alert_rect);
            MMI_GetLabelTextByLang(text_id, &text_str);

            text_style.align = ALIGN_HVMIDDLE;
            text_style.font  = MMIMP3_ALERT_FONT;
            text_style.char_space = char_space;
            text_style.line_space = line_space;
            text_style.font_color = MMIMP3_ALERT_COLOR;

            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T       *)&alert_rect,
                (const GUI_RECT_T       *)&alert_rect,
                (const MMI_STRING_T     *)&text_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
            );
        }
    }
}


/*****************************************************************************/
//  Description : display alert message
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayAlertTips(
    void
)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    text_id = GetMp3AlertTextId(GetMp3AlertType());

    if(TXT_NULL != text_id)
    {
        MMIPUB_DisplayTipsIDEx(PNULL, text_id, MMIPUB_TIPS_DIS_BOTTOM, PNULL);

        SetMp3AlertType(MMIMP3_ALERT_MSG_MAX);
        StartMp3AlertTimer();
    }
}

/*****************************************************************************/
//  Description : handle mp3 alert info display.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_HandleMp3AlertMsg(
    MMIMP3_ALERT_MSG_E      alert_type,
    MMIMP3_ALERT_CALLBACK   alert_callback
)
{
    MMI_WIN_ID_T  win_id = MMIMP3_MAIN_PLAY_WIN_ID;

    if(MMK_IsOpenWin(win_id))
    {
        SetMp3AlertInfo(alert_type, alert_callback, win_id);
        StartMp3AlertTimer();
    }

    if(MMK_IsFocusWin(win_id))
    {
        MMIMP3_DisplayMp3PlayWin(TRUE, win_id);
    }

    //pda使用PubWin直接提示
    {
        MMI_TEXT_ID_T text_id = TXT_NULL;

        if(MMK_IsFocusWin(win_id))
        {
            text_id = GetMp3AlertTextId(alert_type);
            SetMp3AlertType(MMIMP3_ALERT_MSG_MAX);

            if(PNULL != text_id)
            {
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
				//mini 项目，采用Tips不能完全显示字符串信息
                //MMIPUB_DisplayTipsIDEx(PNULL, text_id, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
				MMIPUB_OpenAlertWarningWin(text_id);
#else
                MMIPUB_DisplayTipsIDEx(PNULL, text_id, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
#endif
            }
        }
    }
}

/*****************************************************************************/
//  Description : clear mp3 alert info.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearMp3AlertInfo(
    BOOLEAN exe_callback
)
{
    MMIAP_ALERT_INFO_T alert_info = {0};


    if(exe_callback)
    {
        GetMp3AlertInfo(&alert_info);
        if(PNULL != alert_info.alert_callback)
        {
            alert_info.alert_callback();
        }
    }

    SetMp3AlertInfo(MMIMP3_ALERT_MSG_MAX, NULL, NULL);
    StopMp3AlertTimer();
}

/*****************************************************************************/
//  Description : handle mp3 alert timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_HandleMp3AlertTimer(
    uint8 timer_id
)
{
    MMI_WIN_ID_T  win_id = MMIMP3_MAIN_PLAY_WIN_ID;

    if(GetAPAlertTimerID() == timer_id)
    {
        //SCI_TRACE_LOW:"[MMIMP3]: HandleMp3AlertTimer enter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_4597_112_2_18_2_4_46_309, (uint8*)"");

        MMIMP3_ClearMp3AlertInfo(TRUE);

        if(MMK_IsFocusWin(win_id))
        {
            MMIMP3_DisplayMp3PlayWin(TRUE, win_id);
        }

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : display bg image
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DisplayBackgroudImg(
    MMI_WIN_ID_T win_id
)
{
    GUI_RECT_T          disp_rect   = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    dev_info    = {0};

    GUIRES_DisplayImg(PNULL,
                      &disp_rect,
                      &disp_rect,
                      win_id,
                      IMAGE_MP3_ALBUM_BG,
                      &dev_info
                     );
}

/*****************************************************************************/
//  Description : init common win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitCommonWinInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    ret = CreateAPCommonWinInfo();
    if (!ret)
    {
        return ret;
    }


    SetMp3AlertType(MMIMP3_ALERT_MSG_MAX);

#ifdef CMCC_UI_STYLE
    MMIAP_SetOpenMp3Entry(MMIMP3_ENTRY_TYPE_MAX);
#endif

    return ret;
}

/*****************************************************************************/
//  Description : 获取指定LIST项的文本信息
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetListItemTextInfo(
    MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
    uint16        index,      //[in]  索引
    MMI_STRING_T  *main_text,  //[out] 主标题
    MMI_STRING_T  *sub_text    //[out] 副标题
)
{
    BOOLEAN             return_code = FALSE;
    const GUILIST_ITEM_T      *item_t_ptr = PNULL;
    const GUIITEM_STYLE_T     *style_t_ptr = PNULL;
    int16               main_index = 0;
    int16               sub_index = 0;

    item_t_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);

    if(PNULL != item_t_ptr)
    {
        //通过style获取文本数据的索引位置
        style_t_ptr = MMITHEME_GetItemStyle(item_t_ptr->item_style);

        if(PNULL != style_t_ptr)
        {
            //主文本信息
            main_index = style_t_ptr->main_index;

            if(main_index >= 0)
            {
                main_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_ptr;
                main_text->wstr_len = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_len;

                return_code = TRUE;
            }

            //副文本信息
            sub_index = style_t_ptr->auxiliary_index;

            if(sub_index >= 0)
            {
                sub_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_ptr;
                sub_text->wstr_len = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_len;
            }
        }
    }

    return return_code;
}

/*****************************************************************************/
//  Description : handle Folder Full
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_FolderFull(MMIFILE_DEVICE_E dev)
{
    MMI_WIN_ID_T alert_win_id = MMIMP3_FOLDER_FULL_ALERT_WIN_ID;

    if(!MMK_IsOpenWin(alert_win_id))
    {
        MMIAPIFMM_PromptFileErrorType(alert_win_id, SFS_ERROR_FOLD_FULL, dev);
    }
}

