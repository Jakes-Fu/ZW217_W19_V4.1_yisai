/*****************************************************************************
** File Name:      mmiapwin_main_mini.c                                         *
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

#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT

#define _MMIAPWIN_MAIN_MINI_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **-------------------------------------------------------------------------*/
#include "mmipub.h"
#include "mmidisplay_data.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guirichtext.h"

#include "guilabel.h"
#include "guitext.h"
#include "mmi_appmsg.h"

#include "guiref_scale.h"

#include "mmi_default.h"
#include "mmk_timer.h"
#include "wma_adp.h"
#include "guiedit.h"
#include "guistring.h"

#include "guiownerdraw.h"
#include "guibutton.h"
#include "guistatusbar.h"


#include "mmimultim_image.h"
#include "mmi_text.h"

//other module
#include "mmisrvaud_api.h"
#include "mmifmm_internal.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"
#include "mmiidle_subwintab.h"
#include "mmiidle_export.h"
#include "mmibt_export.h"
#include "mmiset_export.h"
#include "mmi_mainmenu_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmimms_export.h"

#include "mmialarm_export.h"
#include "mmialarm_text.h"

#if defined(DRM_SUPPORT)
#include "drm_api.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm_text.h"
#include "mmidrm_export.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "mmicc_export.h"
#include "mmiacc_text.h"

//local module
#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_MUSIC_LIST_SUPPORT
#include "mmiapwin_multilist.h"
#endif

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#include "mmiap_common.h"
#include "mmimp3_text.h"
#include "mmimp3_anim.h"
#include "mmimp3_id.h"
#include "mmimp3_image.h"
#include "mmiap_position.h"
#include "mmiap_control.h"
#include "mmiapwin_musicmark.h"
#include "mmiap_applet.h"
#include "mmiapwin_set.h"
#include "mmiapwin_list.h"
#include "mmiap_list.h"
#include "mmiap_setting.h"
#include "mmiapwin_main.h"
#include "mmimp3_export.h"
#include "mmiap_appmain.h"
#include "mmiapwin_common.h"

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

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//32X16_128x160  32X16_176x220  32X16_220x176 32X16_160x128
LOCAL WINDOW_TABLE(MMIMP3_PLAY_WIN_TAB_H) =
{
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_PREV, MMIMP3_PREV_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_PLAY, MMIMP3_PLAY_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_NEXT, MMIMP3_NEXT_BTN_CTRL_ID),

//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_DUMMY_LABLE_CTRL_ID),
//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_TITLE_LABLE_CTRL_ID),
//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIMP3_FILE_INFO_LABLE_CTRL_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIMP3_ARTIST_LABLE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIMP3_ALBUM_LABLE_CTRL_ID),

    CREATE_OWNDRAW_CTRL(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CREATE_OWNDRAW_CTRL(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),

    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_SHUFFLE_ON, MMIMP3_SHUFFLE_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_REPEAT_ON, MMIMP3_REPEAT_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_MP3_DEFAULT_ALBUM_IMG, MMIMP3_PIC_BTN_CTRL_ID),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_MP3_BTN_REVIEW, MMIMP3_REVIEW_BTN_CTRL_ID),
#endif
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

	WIN_TITLE(TXT_NULL),

    END_WIN
};


LOCAL WINDOW_TABLE(MMIMP3_PLAY_WIN_TAB_V) =
{
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_PREV, MMIMP3_PREV_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_PLAY, MMIMP3_PLAY_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_NEXT, MMIMP3_NEXT_BTN_CTRL_ID),

//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_DUMMY_LABLE_CTRL_ID),
//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_TITLE_LABLE_CTRL_ID),
//     CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIMP3_FILE_INFO_LABLE_CTRL_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIMP3_ARTIST_LABLE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIMP3_ALBUM_LABLE_CTRL_ID),

    CREATE_OWNDRAW_CTRL(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CREATE_OWNDRAW_CTRL(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),


    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_SHUFFLE_ON, MMIMP3_SHUFFLE_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_REPEAT_ON, MMIMP3_REPEAT_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_MP3_DEFAULT_ALBUM_IMG, MMIMP3_PIC_BTN_CTRL_ID),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_MP3_BTN_REVIEW, MMIMP3_REVIEW_BTN_CTRL_ID),
#endif
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

	WIN_TITLE(TXT_NULL),

    END_WIN
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : set mp3 layout for 32X16_220X and 32x16_128X160
//Global resource dependence :
//Author: yanxian.zhou
//Note:
/*****************************************************************************/
PUBLIC void SetMp3LayoutParam(void)
{
    GUI_BG_T bg = {0};
    GUI_BG_T title_bg = {0};    
    GUI_BG_T gray_bg = {0};

    GUI_RECT_T pre_rect_v = MP3_LAYOUT_PREV_RECT_V;
    GUI_RECT_T play_rect_v = MP3_LAYOUT_PLAY_RECT_V;
    GUI_RECT_T next_rect_v = MP3_LAYOUT_NEXT_RECT_V;

    GUI_RECT_T titledum_rect_v = MP3_TITLE_DUMMY_RECT_V;
    GUI_RECT_T titletext_rect_v = MP3_TITLE_TEXT_RECT_V;
    GUI_RECT_T titlelist_rect_v = MP3_TITLE_INFO_RECT_V;

    GUI_RECT_T artist_rect_v = MP3_ARTIST_RECT_V;
    GUI_RECT_T album_rect_v = MP3_ALBUM_RECT_V;

    GUI_RECT_T progress_rect_v = MP3_PROGRESS_RECT_V;
    GUI_RECT_T time_rect_v = MP3_TIME_RECT_V;

    GUI_RECT_T shuffle_rect_v = MP3_SHUFFLE_RECT_V;
    GUI_RECT_T repeat_rect_v = MP3_REPEAT_RECT_V;
    GUI_RECT_T pic_rect_v = MP3_PIC_RECT_V;
    GUI_RECT_T rev_rect_v = MP3_REV_RECT_V;

//#if defined MAINLCD_DEV_SIZE_220X176 || defined MAINLCD_DEV_SIZE_160X128
    GUI_RECT_T pre_rect_h = MP3_LAYOUT_PREV_RECT_H;
    GUI_RECT_T play_rect_h = MP3_LAYOUT_PLAY_RECT_H;
    GUI_RECT_T next_rect_h = MP3_LAYOUT_NEXT_RECT_H;

    GUI_RECT_T titledum_rect_h = MP3_TITLE_DUMMY_RECT_H;
    GUI_RECT_T titletext_rect_h = MP3_TITLE_TEXT_RECT_H;
    GUI_RECT_T titlelist_rect_h = MP3_TITLE_INFO_RECT_H;

    GUI_RECT_T artist_rect_h = MP3_ARTIST_RECT_H;
    GUI_RECT_T album_rect_h = MP3_ALBUM_RECT_H;

    GUI_RECT_T progress_rect_h = MP3_PROGRESS_RECT_H;
    GUI_RECT_T time_rect_h = MP3_TIME_RECT_H;

    GUI_RECT_T shuffle_rect_h = MP3_SHUFFLE_RECT_H;
    GUI_RECT_T repeat_rect_h = MP3_REPEAT_RECT_H;
    GUI_RECT_T pic_rect_h = MP3_PIC_RECT_H;
    GUI_RECT_T rev_rect_h = MP3_REV_RECT_H;
//#endif

    GUI_RECT_T pre_rect = {0};
    GUI_RECT_T play_rect = {0};
    GUI_RECT_T next_rect = {0};

    GUI_RECT_T tidum_rect = {0};
    GUI_RECT_T titext_rect = {0};
    GUI_RECT_T teinf_rect = {0};

    GUI_RECT_T artist_rect = {0};
    GUI_RECT_T album_rect = {0};

    GUI_RECT_T progress_rect = {0};
    GUI_RECT_T time_rect = {0};

    GUI_RECT_T shuffle_rect = {0};
    GUI_RECT_T repeat_rect = {0};
    GUI_RECT_T pic_rect = {0};
    GUI_RECT_T rev_rect = {0};
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetHandleLong(MMIMP3_PREV_BTN_CTRL_ID, TRUE);
    GUIBUTTON_SetHandleLong(MMIMP3_NEXT_BTN_CTRL_ID, TRUE);

    GUIBUTTON_SetCallBackFunc(MMIMP3_REPEAT_BTN_CTRL_ID,    MMIAP_RepeatBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_SHUFFLE_BTN_CTRL_ID,   MMIAP_ShuffleBtnCallback);
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetCallBackFunc(MMIMP3_EQ_BTN_CTRL_ID,        MMIAP_EqBtnCallback);
#endif
    //    GUIBUTTON_SetCallBackFunc(MMIMP3_VOLUME_BTN_CTRL_ID,    MMIAP_VolumeBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_PREV_BTN_CTRL_ID,      MMIAP_PrevBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_PLAY_BTN_CTRL_ID,      MMIAP_PlayBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_NEXT_BTN_CTRL_ID,      MMIAP_NextBtnCallback);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    GUIBUTTON_SetCallBackFunc(MMIMP3_REVIEW_BTN_CTRL_ID,    MMIAP_ReviewBtnCallback);
#endif

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIBUTTON_SetCallBackFunc(MMIMP3_OPT_BTN_CTRL_ID,       MMIAP_OptBtnCallback);
        GUIBUTTON_SetCallBackFunc(MMIMP3_RETRUN_BTN_CTRL_ID,    MMIAP_ReturnBtnCallback);
    }

#endif
    gray_bg.bg_type = GUI_BG_IMG;
    gray_bg.img_id  = IMAGE_COMMON_BTN_PREV_GREY;
    GUIBUTTON_SetGrayedFg(MMIMP3_PREV_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_PLAY_GREY;
    GUIBUTTON_SetGrayedFg(MMIMP3_PLAY_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_NEXT_GREY;
    GUIBUTTON_SetGrayedFg(MMIMP3_NEXT_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);

    gray_bg.img_id = IMAGE_COMMON_BTN_BG;
    GUIBUTTON_SetGrayed(MMIMP3_PREV_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    GUIBUTTON_SetGrayed(MMIMP3_PLAY_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    GUIBUTTON_SetGrayed(MMIMP3_NEXT_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
#ifndef MMI_RES_LOW_COST
    gray_bg.img_id  = IMAGE_COMMON_BTN_SHUFFLE_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_SHUFFLE_BTN_CTRL_ID, FALSE, &gray_bg, FALSE);
#endif
    GUIBUTTON_SetRunSheen(MMIMP3_SHUFFLE_BTN_CTRL_ID,   FALSE);
#ifndef MMI_RES_LOW_COST
    gray_bg.img_id  = IMAGE_COMMON_BTN_REPEAT_GREY;
#endif
    GUIBUTTON_SetGrayed(MMIMP3_REPEAT_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);
    GUIBUTTON_SetRunSheen(MMIMP3_REPEAT_BTN_CTRL_ID,   FALSE);

#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    gray_bg.bg_type = GUI_BG_IMG;
    gray_bg.img_id  = IMAGE_MP3_BTN_REVIEW_GREY;
    GUIBUTTON_SetGrayedFg(MMIMP3_REVIEW_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);

    gray_bg.img_id = IMAGE_MP3_ALBUM_BG;
    GUIBUTTON_SetGrayed(MMIMP3_REVIEW_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);

    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_MP3_ALBUM_BG;
    GUIBUTTON_SetBg(MMIMP3_REVIEW_BTN_CTRL_ID,   &bg);
#endif

    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_MP3_ALBUM_BG;
    GUIBUTTON_SetBg(MMIMP3_REPEAT_BTN_CTRL_ID,    &bg);
    GUIBUTTON_SetBg(MMIMP3_SHUFFLE_BTN_CTRL_ID,   &bg);


    bg.img_id = IMAGE_COMMON_BTN_BG;
    GUIBUTTON_SetBg(MMIMP3_PREV_BTN_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIMP3_PLAY_BTN_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIMP3_NEXT_BTN_CTRL_ID, &bg);

    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_MP3_OSD_BG_DOWN;

    GUIOWNDRAW_SetBg(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, &bg);
    GUIOWNDRAW_SetBg(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, &bg);

    title_bg.bg_type = GUI_BG_IMG;
    title_bg.img_id = IMAGE_MP3_TITLE_BG;

//     GUILABEL_SetBg(MMIMP3_DUMMY_LABLE_CTRL_ID, &title_bg);
//     GUILABEL_SetBg(MMIMP3_TITLE_LABLE_CTRL_ID, &title_bg);
//     GUILABEL_SetBg(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &title_bg);

    if(!MMITHEME_IsMainScreenLandscape())
    {
        pre_rect = pre_rect_v;
        play_rect = play_rect_v;
        next_rect = next_rect_v;

        tidum_rect = titledum_rect_v;
        titext_rect = titletext_rect_v;
        teinf_rect = titlelist_rect_v;

        artist_rect = artist_rect_v;
        album_rect = album_rect_v;

        progress_rect = progress_rect_v;
        time_rect = time_rect_v;

        shuffle_rect = shuffle_rect_v;
        repeat_rect = repeat_rect_v;
        pic_rect = pic_rect_v;
        rev_rect = rev_rect_v;

    }
    else
    {
//#if defined MAINLCD_DEV_SIZE_220X176 || defined MAINLCD_DEV_SIZE_160X128
        pre_rect = pre_rect_h;
        play_rect = play_rect_h;
        next_rect = next_rect_h;

        tidum_rect = titledum_rect_h;
        titext_rect = titletext_rect_h;
        teinf_rect = titlelist_rect_h;

        artist_rect = artist_rect_h;
        album_rect = album_rect_h;

        progress_rect = progress_rect_h;
        time_rect = time_rect_h;

        shuffle_rect = shuffle_rect_h;
        repeat_rect = repeat_rect_h;
        pic_rect = pic_rect_h;
        rev_rect = rev_rect_h;
//#endif
    }

    GUIBUTTON_SetRect(MMIMP3_SHUFFLE_BTN_CTRL_ID, &shuffle_rect);
    GUIBUTTON_SetRect(MMIMP3_REPEAT_BTN_CTRL_ID, &repeat_rect);
    GUIBUTTON_SetRect(MMIMP3_PIC_BTN_CTRL_ID, &pic_rect);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    GUIBUTTON_SetRect(MMIMP3_REVIEW_BTN_CTRL_ID, &rev_rect);
#endif

    GUIBUTTON_SetRect(MMIMP3_PREV_BTN_CTRL_ID, &pre_rect);
    GUIBUTTON_SetRect(MMIMP3_PLAY_BTN_CTRL_ID, &play_rect);
    GUIBUTTON_SetRect(MMIMP3_NEXT_BTN_CTRL_ID, &next_rect);

    GUILABEL_SetRect(MMIMP3_ARTIST_LABLE_CTRL_ID, &artist_rect, FALSE);
    GUILABEL_SetRect(MMIMP3_ALBUM_LABLE_CTRL_ID, &album_rect, FALSE);

    GUILABEL_SetFont(MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_ALBUM_ARTIST_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetFont(MMIMP3_ALBUM_LABLE_CTRL_ID, MMIMP3_ALBUM_ARTIST_FONT, MMI_WHITE_COLOR);

    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_MP3_ALBUM_BG;
    GUILABEL_SetBg(MMIMP3_ARTIST_LABLE_CTRL_ID, &bg);
    GUILABEL_SetBg(MMIMP3_ALBUM_LABLE_CTRL_ID, &bg);

//     GUILABEL_SetRect(MMIMP3_DUMMY_LABLE_CTRL_ID, &tidum_rect, FALSE);
//     GUILABEL_SetRect(MMIMP3_TITLE_LABLE_CTRL_ID, &titext_rect, FALSE);
//     GUILABEL_SetRect(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &teinf_rect, FALSE);
// 
//     GUILABEL_SetFont(MMIMP3_TITLE_LABLE_CTRL_ID, MMIMP3_TITLE_FONT, MMI_WHITE_COLOR);
//     GUILABEL_SetFont(MMIMP3_FILE_INFO_LABLE_CTRL_ID, MMIMP3_SUB_TEXT_FONT, MMI_WHITE_COLOR);
// 
//     GUILABEL_SetVisible(MMIMP3_FILE_INFO_LABLE_CTRL_ID, TRUE, FALSE);
	GUITITLE_SetTextAlignType(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), ALIGN_HMIDDLE);

    GUIOWNDRAW_SetRect(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, &progress_rect);
    GUIOWNDRAW_SetRect(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, &time_rect);


    MMIAP_SetOsdPanelFlag(TRUE);
    {
        MMI_STRING_T tip_text = {0};
        wchar        tip_wstr[MMIMP3_DISPLAY_TIME_LEN + 1] = {0};
        MMIAP_GetOsdFileTipInfo(tip_wstr, MMIMP3_DISPLAY_TIME_LEN); /*lint !e774*/
        tip_text.wstr_ptr = tip_wstr;
        tip_text.wstr_len = MMIAPICOM_Wstrlen(tip_wstr);

		GUITITLE_SetSubText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), TRUE,tip_text.wstr_ptr, tip_text.wstr_len, TRUE);
 //       GUILABEL_SetFont(MMIMP3_FILE_INFO_LABLE_CTRL_ID, MMIMP3_SUB_TEXT_FONT, MMI_WHITE_COLOR);
    }

    {
        MMI_CTRL_ID_T   ctrl_id1 = MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID;
        MMI_CTRL_ID_T   ctrl_id2 = MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID;

        GUIOWNDRAW_Update(ctrl_id1);
        GUIOWNDRAW_Update(ctrl_id2);
    }

}

/*****************************************************************************/
//  Description : join main mini win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void JoinMainMiniWin(
    MMI_WIN_ID_T  win_id //in
)
{
    if (0 != win_id)
    {
        MMK_CreateCtrlByWinTabEx(win_id,  MMITHEME_IsMainScreenLandscape() ? MMIMP3_PLAY_WIN_TAB_H : MMIMP3_PLAY_WIN_TAB_V);  /*lint !e605*/
    }
}

/*****************************************************************************/
//  Description : handle mini open window message
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMiniDefaultOpenWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMIAP_MAIN_WIN_INFO_T *win_info_ptr = (MMIAP_MAIN_WIN_INFO_T*)MMK_GetWinUserData(win_id);


    if (PNULL != win_info_ptr
            && PNULL != win_info_ptr->func_info.Func_DefaultOpenWinMsg
       )
    {
        result = win_info_ptr->func_info.Func_DefaultOpenWinMsg(win_id, msg_id, param);
    }


    return result;
}

/*****************************************************************************/
//  Description : handle mini default window message
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMiniDefaultWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;


    SCI_TRACE_LOW("[MMIAP] HandleMiniDefaultWinMsg msg_id=0x%X",
                  msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetMp3LayoutParam();

        HandleMiniDefaultOpenWinMsg(win_id, msg_id, param);

//        MMK_SetAtvCtrl(win_id, MMIMP3_TITLE_LABLE_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
        MMIAP_HandleAppWebKeyWinMsg(win_id, msg_id, param);
        break;
    case MSG_LCD_SWITCH:
        MMIAPMAINWIN_StopTimers();

        JoinMainMiniWin(win_id);
        SetMp3LayoutParam();

        MMIAPMAINWIN_UpdatePlaySurface();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }


    SCI_TRACE_LOW("[MMIAP] HandleMiniDefaultWinMsg result=%d",
                  result);

    return result;
}

/*****************************************************************************/
//  Description : handle applet resume
//  Global resource dependence :
//  Author:robert.wang
//  Note: default process by returned false
/*****************************************************************************/
LOCAL void HandleMiniAppletResume(
    MMI_WIN_ID_T win_id
)
{
    JoinMainMiniWin(win_id);

    SetMp3LayoutParam();
}

/*****************************************************************************/
//  Description : init win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_InitSubWin(
    MMIAP_WIN_FUNC_INFO_T *func_ptr //in
)
{
    if (PNULL == func_ptr)
    {
        return;
    }

    func_ptr->Func_CreateMainWin = JoinMainMiniWin;
    func_ptr->Func_SubWinMsg = HandleMiniDefaultWinMsg;

    func_ptr->Func_AppletResume = HandleMiniAppletResume;
}
#endif //#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
