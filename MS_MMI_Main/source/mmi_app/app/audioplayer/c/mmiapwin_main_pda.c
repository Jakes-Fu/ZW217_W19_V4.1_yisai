/*****************************************************************************
** File Name:      mmiapwin_main_pda.c                                       *
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

#define _MMIAPWIN_MAIN_PDA_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **-------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"

#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmi_appmsg.h"


//gui & mmk
#include "guilistbox.h"
#include "guirichtext.h"
#include "guitext.h"
#include "guiedit.h"
#include "guistring.h"
#include "guiref_scale.h"
#include "guires.h"
#include "guiownerdraw.h"
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
#include "mmisrvaud_api.h"
#include "mmi_mainmenu_export.h"

#include "mmialarm_export.h"
#include "mmialarm_text.h"

#include "mmiset_export.h"
#include "mmiacc_text.h"
#include "mmiidle_export.h"

#include "mmifmm_internal.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"

#include "mmiidle_subwintab.h"
#include "mmicc_export.h"

#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmimms_export.h"


#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#if defined(DRM_SUPPORT)
#include "drm_api.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm_text.h"
#include "mmidrm_export.h"
#endif


//local module
#include "mmiap_position.h"
#include "mmimp3_image.h"
#include "mmimp3_export.h"
#include "mmimp3_text.h"
#include "mmimp3_anim.h"
#include "mmiap_common.h"
#include "mmiapwin_common.h"
#include "mmimp3_id.h"
#include "mmiap_control.h"
#include "mmiap_applet.h"
#include "mmiapwin_set.h"
#include "mmiapwin_list.h"
#include "mmiap_list.h"
#include "mmiap_setting.h"
#include "mmiapwin_main.h"
#include "mmiap_appmain.h"
#include "mmiap_play.h"

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

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#include "mmiapwin_musicmark.h"
#endif

#include "mmiapwin_common.h"
#include "mmiapwin_menu.h"

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

/*****************************************************************************/
//  Description : SetMp3CtrlParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetMp3CtrlParam(MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//PDA 新界面 or TK 界面
LOCAL WINDOW_TABLE(MMIMP3_PLAY_WIN_TAB_H) =
{
    WIN_STATUSBAR,

    CREATE_ANIM_CTRL(MMIMP3_PRE_ALBUM_ANIM_CTRL_ID,  MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_TITLE_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ALBUM_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),

    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, MMIMP3_PANEL_H_FORM_CTRL_ID,  PNULL),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_COMMON_BTN_LIST,            MMIMP3_LIST_BTN_CTRL_ID,   MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY2_OWNER_DRAW_CTRL_ID,  MMIMP3_BTN_FORM_CTRL1_ID,  PNULL),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_COMMON_PLAYER_BTN_VOL,      MMIMP3_VOLUME_BTN_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_BTN_FORM_FATHER_ID),
    //    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMP3_BTN_FORM_DUMMY_ID,MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV,         MMIMP3_PREV_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PLAY,         MMIMP3_PLAY_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT,         MMIMP3_NEXT_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_BTN_OWNER_DRAW_CTRL_ID, MMIMP3_BTN_FORM_CTRL2_ID, PNULL),

    //浮动面板
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_FILE_INFO_LABLE_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,   MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_SHUFFLE_ON,     MMIMP3_SHUFFLE_BTN_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_REPEAT_ON,      MMIMP3_REPEAT_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY3_OWNER_DRAW_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID,  PNULL),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_BTN_REVIEW,            MMIMP3_REVIEW_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID,  MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),

    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_PLAY_WIN_TAB_V) =
{
    WIN_STATUSBAR,

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_LIST, MMIMP3_LIST_BTN_CTRL_ID,   MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_TITLE_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ALBUM_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIMP3_VOLUME_BTN_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),

    CREATE_ANIM_CTRL(MMIMP3_PRE_ALBUM_ANIM_CTRL_ID,  MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_BTN_FORM_FATHER_ID),
    //    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMP3_BTN_FORM_DUMMY_ID,MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV,         MMIMP3_PREV_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PLAY,         MMIMP3_PLAY_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT,         MMIMP3_NEXT_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_BTN_OWNER_DRAW_CTRL_ID, MMIMP3_BTN_FORM_CTRL2_ID, PNULL),


    //浮动面板
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_FILE_INFO_LABLE_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,   MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_SHUFFLE_ON,   MMIMP3_SHUFFLE_BTN_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_REPEAT_ON,    MMIMP3_REPEAT_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID,  PNULL),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_BTN_REVIEW,          MMIMP3_REVIEW_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),

    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
//Istyle风格
LOCAL WINDOW_TABLE(MMIMP3_ISTYLE_PLAY_WIN_TAB_H) =
{
    WIN_STATUSBAR,

    CREATE_ANIM_CTRL(MMIMP3_PRE_ALBUM_ANIM_CTRL_ID,  MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_PANEL_H_FORM_CTRL_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_ISTYLE_BTN_FORM_CTRL_ID, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_MP3_ISTYLE_BTN_RETURN,            MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID, MMIMP3_ISTYLE_BTN_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_ISTYLE_DUMY_OWNER_DRAW_CTRL_ID,  MMIMP3_ISTYLE_BTN_FORM_CTRL_ID, PNULL),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_MP3_ISTYLE_BTN_OPTION,            MMIMP3_ISTYLE_OPT_BTN_CTRL_ID, MMIMP3_ISTYLE_BTN_FORM_CTRL_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_TITLE_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ALBUM_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),

    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, MMIMP3_PANEL_H_FORM_CTRL_ID,  PNULL),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_PANEL_H_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_COMMON_BTN_LIST,            MMIMP3_LIST_BTN_CTRL_ID,   MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY2_OWNER_DRAW_CTRL_ID,  MMIMP3_BTN_FORM_CTRL1_ID,  PNULL),
    CHILD_BUTTON_CTRL(FALSE,  IMAGE_COMMON_PLAYER_BTN_VOL,      MMIMP3_VOLUME_BTN_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_BTN_FORM_FATHER_ID),
    //    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMP3_BTN_FORM_DUMMY_ID,MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV,         MMIMP3_PREV_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PLAY,         MMIMP3_PLAY_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT,         MMIMP3_NEXT_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_OWNDRAW_CTRL(FALSE,MMIMP3_BTN_OWNER_DRAW_CTRL_ID, MMIMP3_BTN_FORM_CTRL2_ID,PNULL),


    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_FILE_INFO_LABLE_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,   MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_SHUFFLE_ON,     MMIMP3_SHUFFLE_BTN_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_REPEAT_ON,      MMIMP3_REPEAT_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY3_OWNER_DRAW_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID,  PNULL),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_BTN_REVIEW,            MMIMP3_REVIEW_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID,  MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),

    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_ISTYLE_PLAY_WIN_TAB_V) =
{
    WIN_STATUSBAR,

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_ISTYLE_BTN_RETURN, MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID,   MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_TITLE_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIMP3_ALBUM_LABLE_CTRL_ID,  MMIMP3_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_ISTYLE_BTN_OPTION, MMIMP3_ISTYLE_OPT_BTN_CTRL_ID, MMIMP3_BTN_FORM_CTRL1_ID),

    CREATE_ANIM_CTRL(MMIMP3_PRE_ALBUM_ANIM_CTRL_ID,  MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),
    CREATE_ANIM_CTRL(MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID, MMIMP3_MAIN_PLAY_WIN_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_BTN_FORM_FATHER_ID),
    //    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMP3_BTN_FORM_DUMMY_ID,MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_FORM_FATHER_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_LIST,         MMIMP3_LIST_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV,         MMIMP3_PREV_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PLAY,         MMIMP3_PLAY_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT,         MMIMP3_NEXT_BTN_CTRL_ID,    MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL,   MMIMP3_VOLUME_BTN_CTRL_ID,  MMIMP3_BTN_FORM_CTRL2_ID),
    CHILD_OWNDRAW_CTRL(FALSE,MMIMP3_BTN_OWNER_DRAW_CTRL_ID, MMIMP3_BTN_FORM_CTRL2_ID,PNULL),


    //浮动面板
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMP3_FILE_INFO_LABLE_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,   MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_SHUFFLE_ON,   MMIMP3_SHUFFLE_BTN_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_REPEAT_ON,    MMIMP3_REPEAT_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID,  PNULL),
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_MP3_BTN_REVIEW,          MMIMP3_REVIEW_BTN_CTRL_ID,  MMIMP3_OSD_BTN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_OSD_PANEL_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawProgressOwnerDrawCtrl),
    CHILD_OWNDRAW_CTRL(FALSE, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, (uint32)MMIAP_DrawTimeOwnerDrawCtrl),

    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

#endif //MMI_PDA_SUPPORT


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : join main pda win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void JoinMainPDAWin(
    MMI_WIN_ID_T  win_id //in
)
{
    if (0 != win_id)
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 

        if(MMITHEME_IsIstyle())
        {
            MMK_CreateCtrlByWinTabEx(win_id,  MMITHEME_IsMainScreenLandscape() ? MMIMP3_ISTYLE_PLAY_WIN_TAB_H : MMIMP3_ISTYLE_PLAY_WIN_TAB_V);  /*lint !e605*/
        }
        else
#endif
        {
            MMK_CreateCtrlByWinTabEx(win_id,  MMITHEME_IsMainScreenLandscape() ? MMIMP3_PLAY_WIN_TAB_H : MMIMP3_PLAY_WIN_TAB_V);  /*lint !e605*/
        }
    }
}

/*****************************************************************************/
//  Description : handle main open window message
//  Global resource dependence :
//  Author:robert.wang
//  Note: default process by returned false
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainDefaultOpenWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMIAP_MAIN_WIN_INFO_T *win_info_ptr = (MMIAP_MAIN_WIN_INFO_T*)MMK_GetWinUserData(win_id);


    if (PNULL != win_info_ptr)
    {
        if (PNULL != win_info_ptr->func_info.Func_DefaultOpenWinMsg
           )
        {
            result = win_info_ptr->func_info.Func_DefaultOpenWinMsg(win_id, msg_id, param);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : handle pda default window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePDADefaultWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;


    SCI_TRACE_LOW("[MMIAP] HandlePDADefaultWinMsg msg_id=0x%X",
                  msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMP3_NoSupportNumClear();
        SetMp3CtrlParam(win_id);

        HandleMainDefaultOpenWinMsg(win_id, msg_id, param);
        break;
    case MSG_APP_WEB:
        MMIAP_HandleAppWebKeyWinMsg(win_id, msg_id, param);
        break;
    case MSG_LCD_SWITCH:
        MMIAPMAINWIN_StopTimers();

        JoinMainPDAWin(win_id);
        SetMp3CtrlParam(win_id);

        MMIAPMAINWIN_UpdatePlaySurface();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }


    SCI_TRACE_LOW("[MMIAP] HandlePDADefaultWinMsg result=%d",
                  result);

    return result;
}


/*****************************************************************************/
//  Description : handle applet resume
//  Global resource dependence :
//  Author:robert.wang
//  Note: default process by returned false
/*****************************************************************************/
LOCAL void HandlePDAAppletResume(
    MMI_WIN_ID_T win_id
)
{

    JoinMainPDAWin(win_id);

    SetMp3CtrlParam(win_id);
}

#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT

/*****************************************************************************/
//  Description : init sub win
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

    func_ptr->Func_CreateMainWin = JoinMainPDAWin;
    func_ptr->Func_SubWinMsg = HandlePDADefaultWinMsg;


    func_ptr->Func_AppletResume = HandlePDAAppletResume;
}

#endif

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : istyle return button callback
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IstyleReturnBtnCallback(void)
{
    MMK_PostMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_APP_CANCEL, PNULL, NULL);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : istyle option button callback
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IstyleOptBtnCallback(void)
{
    MMK_PostMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_APP_MENU, PNULL, NULL);
    return MMI_RESULT_TRUE;
}
#endif

/*****************************************************************************/
//  Description : SetMp3BtnParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetMp3BtnParam(void)
{
    GUI_BG_T gray_bg = {0};
    GUI_BG_T press_bg = {0};
    GUI_BG_T press_fg = {0};
    GUI_BG_T normal_bg = {0};

    GUIBUTTON_SetHandleLong(MMIMP3_PREV_BTN_CTRL_ID, TRUE);
    GUIBUTTON_SetHandleLong(MMIMP3_NEXT_BTN_CTRL_ID, TRUE);

    GUIBUTTON_SetCallBackFunc(MMIMP3_REPEAT_BTN_CTRL_ID,    MMIAP_RepeatBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_SHUFFLE_BTN_CTRL_ID,   MMIAP_ShuffleBtnCallback);
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetCallBackFunc(MMIMP3_EQ_BTN_CTRL_ID,        MMIAP_EqBtnCallback);
#endif
    GUIBUTTON_SetCallBackFunc(MMIMP3_VOLUME_BTN_CTRL_ID,    MMIAP_VolumeBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_PREV_BTN_CTRL_ID,      MMIAP_PrevBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_PLAY_BTN_CTRL_ID,      MMIAP_PlayBtnCallback);
    GUIBUTTON_SetCallBackFunc(MMIMP3_NEXT_BTN_CTRL_ID,      MMIAP_NextBtnCallback);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    GUIBUTTON_SetCallBackFunc(MMIMP3_REVIEW_BTN_CTRL_ID,    MMIAP_ReviewBtnCallback);
#endif
    GUIBUTTON_SetCallBackFunc(MMIMP3_LIST_BTN_CTRL_ID,      MMIAP_ListBtnCallback);
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 

    if(MMITHEME_IsIstyle())
    {
        GUIBUTTON_SetCallBackFunc(MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID,     IstyleReturnBtnCallback);
        GUIBUTTON_SetCallBackFunc(MMIMP3_ISTYLE_OPT_BTN_CTRL_ID,        IstyleOptBtnCallback);
    }

#endif

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIBUTTON_SetCallBackFunc(MMIMP3_OPT_BTN_CTRL_ID,       MMIAP_OptBtnCallback);
        GUIBUTTON_SetCallBackFunc(MMIMP3_RETRUN_BTN_CTRL_ID,    MMIAP_ReturnBtnCallback);
    }


    //gray img
    gray_bg.bg_type = GUI_BG_IMG;

    gray_bg.img_id  = IMAGE_COMMON_BTN_REPEAT_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_REPEAT_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_SHUFFLE_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_SHUFFLE_BTN_CTRL_ID, FALSE, &gray_bg, FALSE);
#ifdef TOUCH_PANEL_SUPPORT
    gray_bg.img_id  = IMAGE_COMMON_BTN_EQ;
    GUIBUTTON_SetGrayed(MMIMP3_EQ_BTN_CTRL_ID,      FALSE, &gray_bg, FALSE);
#endif
    gray_bg.img_id  = IMAGE_COMMON_PLAYER_BTN_VOL_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_VOLUME_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_PREV_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_PREV_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_PLAY_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_PLAY_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_NEXT_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_NEXT_BTN_CTRL_ID,    FALSE, &gray_bg, FALSE);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    gray_bg.img_id  = IMAGE_MP3_BTN_REVIEW_GREY;
#endif
    GUIBUTTON_SetGrayed(MMIMP3_REVIEW_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);
    gray_bg.img_id  = IMAGE_COMMON_BTN_LIST_GREY;
    GUIBUTTON_SetGrayed(MMIMP3_LIST_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);

    //pressed fg img

    press_fg.bg_type = GUI_BG_IMG;
    press_fg.img_id  = IMAGE_COMMON_BTN_LIST_FOCUS;
    GUIBUTTON_SetPressedFg(MMIMP3_LIST_BTN_CTRL_ID, &press_fg);
    press_fg.img_id  = IMAGE_COMMON_BTN_LIST;
    GUIBUTTON_SetFg(MMIMP3_LIST_BTN_CTRL_ID, &press_fg);
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 

    if(MMITHEME_IsIstyle())
    {
        press_fg.bg_type = GUI_BG_IMG;

        press_fg.img_id  = IMAGE_MP3_ISTYLE_BTN_RETURN_FOCUS;
        GUIBUTTON_SetPressedFg(MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID, &press_fg);
        press_fg.img_id  = IMAGE_MP3_ISTYLE_BTN_RETURN;
        GUIBUTTON_SetFg(MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID, &press_fg);

        press_fg.img_id  = IMAGE_MP3_ISTYLE_BTN_OPTION_FOCUS;
        GUIBUTTON_SetPressedFg(MMIMP3_ISTYLE_OPT_BTN_CTRL_ID, &press_fg);
        press_fg.img_id  = IMAGE_MP3_ISTYLE_BTN_OPTION;
        GUIBUTTON_SetFg(MMIMP3_ISTYLE_OPT_BTN_CTRL_ID, &press_fg);
    }

#endif

    if(MMITHEME_IsMainScreenLandscape())
    {
        gray_bg.img_id  = IMAGE_COMMON_BTN_OPTION;
        GUIBUTTON_SetGrayed(MMIMP3_OPT_BTN_CTRL_ID,     FALSE, &gray_bg, FALSE);
        gray_bg.img_id  = IMAGE_COMMON_BTN_RETURN;
        GUIBUTTON_SetGrayed(MMIMP3_RETRUN_BTN_CTRL_ID,  FALSE, &gray_bg, FALSE);

        press_bg.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
        press_bg.img_id  = IMAGE_COMMON_BTN_PRESS;
#endif
        GUIBUTTON_SetPressedBg(MMIMP3_OPT_BTN_CTRL_ID, &press_bg);
        GUIBUTTON_SetPressedBg(MMIMP3_RETRUN_BTN_CTRL_ID, &press_bg);
    }

    //btn bg
    normal_bg.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIMP3_REPEAT_BTN_CTRL_ID,    &normal_bg);
    GUIBUTTON_SetBg(MMIMP3_SHUFFLE_BTN_CTRL_ID,   &normal_bg);
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetBg(MMIMP3_EQ_BTN_CTRL_ID,        &normal_bg);
#endif
    GUIBUTTON_SetBg(MMIMP3_VOLUME_BTN_CTRL_ID,    &normal_bg);
    GUIBUTTON_SetBg(MMIMP3_PREV_BTN_CTRL_ID,      &normal_bg);
    GUIBUTTON_SetBg(MMIMP3_PLAY_BTN_CTRL_ID,      &normal_bg);
    GUIBUTTON_SetBg(MMIMP3_NEXT_BTN_CTRL_ID,      &normal_bg);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    GUIBUTTON_SetBg(MMIMP3_REVIEW_BTN_CTRL_ID,    &normal_bg);
#endif

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIBUTTON_SetBg(MMIMP3_OPT_BTN_CTRL_ID,       &normal_bg);
        GUIBUTTON_SetBg(MMIMP3_RETRUN_BTN_CTRL_ID,    &normal_bg);
    }

    //sheen
    GUIBUTTON_SetRunSheen(MMIMP3_REPEAT_BTN_CTRL_ID,    FALSE);
    GUIBUTTON_SetRunSheen(MMIMP3_SHUFFLE_BTN_CTRL_ID,   FALSE);
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetRunSheen(MMIMP3_EQ_BTN_CTRL_ID,        FALSE);
#endif
    GUIBUTTON_SetRunSheen(MMIMP3_VOLUME_BTN_CTRL_ID,    FALSE);
    GUIBUTTON_SetRunSheen(MMIMP3_PREV_BTN_CTRL_ID,      FALSE);
    GUIBUTTON_SetRunSheen(MMIMP3_PLAY_BTN_CTRL_ID,      FALSE);
    GUIBUTTON_SetRunSheen(MMIMP3_NEXT_BTN_CTRL_ID,      FALSE);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    GUIBUTTON_SetRunSheen(MMIMP3_REVIEW_BTN_CTRL_ID,    FALSE);
#endif
    GUIBUTTON_SetRunSheen(MMIMP3_LIST_BTN_CTRL_ID,    FALSE);
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 

    if(MMITHEME_IsIstyle())
    {
        GUIBUTTON_SetRunSheen(MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID, FALSE);
        GUIBUTTON_SetRunSheen(MMIMP3_ISTYLE_OPT_BTN_CTRL_ID,    FALSE);
    }

#endif

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIBUTTON_SetRunSheen(MMIMP3_OPT_BTN_CTRL_ID,       FALSE);
        GUIBUTTON_SetRunSheen(MMIMP3_RETRUN_BTN_CTRL_ID,    FALSE);
    }

    //PUBLIC BOOLEAN GUIBUTTON_SetRunSheen
    //PUBLIC BOOLEAN GUIBUTTON_SetRect
}
#define SOFTKEY_HEIGHT  80
/*****************************************************************************/
//  Description : set pda style form param
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3PdaFormParam(
    MMI_WIN_ID_T win_id //in
)
{
    GUI_BG_T    gui_bg = {0};
    uint16      hor_space = 0;
    uint16      ver_space = 0;
    GUIFORM_CHILD_HEIGHT_T      dummy_height = {0};



    gui_bg.bg_type = GUI_BG_COLOR;
    gui_bg.color = MMI_BLACK_COLOR;
    //设置starusbar的背景
    GUIWIN_SetStbBg(win_id, gui_bg);

    GUIFORM_IsSlide(MMIMP3_MAIN_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMIMP3_MAIN_FORM_CTRL_ID, 0);
    GUIFORM_SetSpace(MMIMP3_MAIN_FORM_CTRL_ID, &hor_space, &ver_space);

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIFORM_CHILD_WIDTH_T   pda_play_btn_width = {0};
        GUIFORM_CHILD_HEIGHT_T  pda_title_total_height = {0};
        GUIFORM_CHILD_HEIGHT_T  pda_title_main_height = {0};
        GUIFORM_CHILD_HEIGHT_T  pda_title_sub_height = {0};
        GUIFORM_CHILD_WIDTH_T   pda_list_btn_width = {0};
        GUIFORM_CHILD_HEIGHT_T  osd_item_height = {0};
        GUIFORM_CHILD_WIDTH_T   osd_item_width = {0};
        GUIFORM_CHILD_HEIGHT_T  istyle_btn_height = {0};
        GUI_RECT_T  file_label_rect = MMIMP3_OSD_LABEL_RECT_H;
        GUI_RECT_T  osd_panel_rect = MMIMP3_OSD_PANEL_RECT_H;
        GUI_RECT_T  album_anim_rect = MMIMP3_ALBUM_ANIM_RECT_H;
        GUI_RECT_T  hor_title_form_rect = MMIMP3_HOR_TITLE_FORM_RECT;
        GUI_RECT_T  hor_play_btn_form_rect = MMIMP3_HOR_PLAY_FORM_RECT;
        uint16 button_num = 0;
        uint16 width_button = 0;
        uint16 button_space = 0;
        uint16 lcd_width = 0;

        //专辑图片区
        GUIANIM_SetCtrlRect(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, &album_anim_rect);

        //右侧面板栏
        GUIFORM_IsSlide(MMIMP3_PANEL_H_FORM_CTRL_ID, FALSE);
        GUIFORM_SetRect(MMIMP3_PANEL_H_FORM_CTRL_ID, &hor_title_form_rect);
        gui_bg.bg_type = GUI_BG_IMG;
        gui_bg.img_id  = IMAGE_MP3_TITLE_BG;
        GUIFORM_SetBg(MMIMP3_PANEL_H_FORM_CTRL_ID, &gui_bg);
        GUIFORM_SetMargin(MMIMP3_PANEL_H_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_PANEL_H_FORM_CTRL_ID, &hor_space, &ver_space);
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 

        //istyle button form
        if(MMITHEME_IsIstyle())
        {
            istyle_btn_height.type      = GUIFORM_CHILD_HEIGHT_FIXED;
            GUIRES_GetImgWidthHeight(PNULL, &istyle_btn_height.add_data, IMAGE_MP3_ISTYLE_BTN_RETURN, win_id);

            GUIFORM_SetChildHeight(MMIMP3_PANEL_H_FORM_CTRL_ID, MMIMP3_ISTYLE_BTN_FORM_CTRL_ID, &istyle_btn_height);

            pda_list_btn_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
            pda_list_btn_width.add_data = MMIMP3_LIST_BTN_WIDTH_H;
            GUIFORM_SetChildWidth(MMIMP3_ISTYLE_BTN_FORM_CTRL_ID, MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID,   &pda_list_btn_width); //list button
            GUIFORM_SetChildWidth(MMIMP3_ISTYLE_BTN_FORM_CTRL_ID, MMIMP3_ISTYLE_OPT_BTN_CTRL_ID, &pda_list_btn_width); //vol  button
        }

#endif
        // title部分
        pda_title_total_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        pda_title_total_height.add_data = MMIMP3_TITLE_TOTAL_HEIGHT_H;
        GUIFORM_SetChildHeight(MMIMP3_PANEL_H_FORM_CTRL_ID, MMIMP3_TITLE_FORM_CTRL_ID, &pda_title_total_height);
        // title元素排版
        pda_title_main_height.type      = GUIFORM_CHILD_HEIGHT_FIXED;
        pda_title_main_height.add_data  = MMIMP3_TITLE_MAIN_HEIGHT_H;
        pda_title_sub_height.type       = GUIFORM_CHILD_HEIGHT_FIXED;
        pda_title_sub_height.add_data   = MMIMP3_TITLE_SUB_HEIGHT_H;
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_ARTIST_LABLE_CTRL_ID, &pda_title_sub_height);
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_TITLE_LABLE_CTRL_ID,  &pda_title_main_height);
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_ALBUM_LABLE_CTRL_ID,  &pda_title_sub_height);

        //填充
#if defined MMI_ISTYLE_SUPPORT         
        if(MMITHEME_IsIstyle())
        {
            GUIOWNDRAW_SetHeight(MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, MMIMP3_ISTYLE_LIST_BTN_UP_SPACE - istyle_btn_height.add_data);
        }
        else
#endif			
        {
            GUIOWNDRAW_SetHeight(MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID, (uint16)MMIMP3_LIST_BTN_UP_SPACE);
        }

        // list、volume button部分
        pda_list_btn_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        pda_list_btn_width.add_data = MMIMP3_LIST_BTN_WIDTH_H;
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_LIST_BTN_CTRL_ID,   &pda_list_btn_width); //list button
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_VOLUME_BTN_CTRL_ID, &pda_list_btn_width); //vol  button

        //播放按钮栏
        GUIFORM_IsSlide(MMIMP3_BTN_FORM_FATHER_ID, FALSE);
        GUIFORM_SetRect(MMIMP3_BTN_FORM_FATHER_ID, &hor_play_btn_form_rect);
        gui_bg.bg_type = GUI_BG_IMG;
#ifdef MMI_PDA_SUPPORT
        gui_bg.img_id  = IMAGE_MP3_H_BTN_BG; //pda下使用自有资源
#else
        gui_bg.img_id  = IMAGE_COMMON_BTN_BG;
#endif
        GUIFORM_SetBg(MMIMP3_BTN_FORM_FATHER_ID, &gui_bg);
        GUIFORM_SetMargin(MMIMP3_BTN_FORM_FATHER_ID, 0);
        GUIFORM_SetSpace(MMIMP3_BTN_FORM_FATHER_ID, &hor_space, &ver_space);

        //        GUIRES_GetImgWidthHeight(&width_button,&height_button,IMAGE_COMMON_BTN_PLAY,win_id);
        //        GUIFORM_IsSlide(MMIMP3_BTN_FORM_DUMMY_ID, FALSE);
        //GUIFORM_SetBg(MMIMP3_BTN_FORM_DUMMY_ID, &gui_bg);
        //        dummy_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        //        dummy_height.add_data =  (hor_play_btn_form_rect.bottom - hor_play_btn_form_rect.top - height_button)/4;
        //        GUIFORM_SetChildHeight(MMIMP3_BTN_FORM_FATHER_ID,MMIMP3_BTN_FORM_DUMMY_ID,&dummy_height);
        //        GUIFORM_SetMargin(MMIMP3_BTN_FORM_DUMMY_ID, 0);
        //        GUIFORM_SetSpace(MMIMP3_BTN_FORM_DUMMY_ID, &hor_space, &ver_space);

        GUIFORM_IsSlide(MMIMP3_BTN_FORM_CTRL2_ID, FALSE);
        GUIFORM_SetBg(MMIMP3_BTN_FORM_CTRL2_ID, &gui_bg);
        GUIFORM_SetMargin(MMIMP3_BTN_FORM_CTRL2_ID, 0);
        GUIFORM_SetAlign(MMIMP3_BTN_FORM_CTRL2_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetSpace(MMIMP3_BTN_FORM_CTRL2_ID, &hor_space, &ver_space);
        // 元素排版
        pda_play_btn_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        //pda_play_btn_width.add_data = MMIMP3_PLAY_BTN_WIDTH_H;
        GUIRES_GetImgWidthHeight(&pda_play_btn_width.add_data, PNULL, IMAGE_COMMON_BTN_PLAY, win_id);
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PREV_BTN_CTRL_ID, &pda_play_btn_width); //pre  button
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PLAY_BTN_CTRL_ID, &pda_play_btn_width); //play button
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_NEXT_BTN_CTRL_ID, &pda_play_btn_width); //next button
        {
            uint16  dumy_owndraw_height = hor_play_btn_form_rect.bottom - hor_play_btn_form_rect.top + 1;

            //填充自绘控件确保button垂直居中，后继FORM将支持垂直居中
            pda_play_btn_width.add_data = 1;
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_OWNER_DRAW_CTRL_ID, &pda_play_btn_width);
            GUIOWNDRAW_SetHeight(MMIMP3_BTN_OWNER_DRAW_CTRL_ID, (dumy_owndraw_height - dummy_height.add_data));
        }

        button_num = 3;
        GUIRES_GetImgWidthHeight(&width_button, PNULL, IMAGE_COMMON_BTN_PLAY, win_id);
        lcd_width = hor_play_btn_form_rect.right - hor_play_btn_form_rect.left + 1;
        button_space = (lcd_width - (width_button * button_num)) / (button_num) - 1;
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PREV_BTN_CTRL_ID, &button_space, PNULL);
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PLAY_BTN_CTRL_ID, &button_space, PNULL);
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_NEXT_BTN_CTRL_ID, &button_space, PNULL);

        //OSD浮动面板
        //up panel
        gui_bg.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
        gui_bg.img_id  = IMAGE_MP3_OSD_BG_UP;
#endif
        GUILABEL_SetRect(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &file_label_rect, FALSE);
        GUILABEL_SetBg(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &gui_bg);

        //down panel
        GUIFORM_IsSlide(MMIMP3_OSD_PANEL_FORM_CTRL_ID, FALSE);
        gui_bg.bg_type = GUI_BG_IMG;
        gui_bg.img_id  = IMAGE_MP3_OSD_BG_DOWN;
        GUIFORM_SetBg(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &gui_bg);
        GUIFORM_SetRect(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &osd_panel_rect);
        GUIFORM_SetMargin(MMIMP3_OSD_PANEL_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &hor_space, &ver_space);

        osd_item_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        osd_item_height.add_data = MMIMP3_OSD_BTN_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_PANEL_FORM_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID, &osd_item_height);

        GUIFORM_SetMargin(MMIMP3_PROCESS_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_PROCESS_FORM_CTRL_ID, &hor_space, &ver_space);
        osd_item_height.add_data = MMIMP3_OSD_PROGRESS_HEIGHT + MMIMP3_OSD_TIME_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_PANEL_FORM_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, &osd_item_height);
        osd_item_height.add_data = MMIMP3_OSD_PROGRESS_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, &osd_item_height);
        osd_item_height.add_data = MMIMP3_OSD_TIME_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, &osd_item_height);

        //osd btn
        osd_item_height.add_data =  MMIMP3_OSD_BTN_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REPEAT_BTN_CTRL_ID,   &osd_item_height); //repeat button
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_SHUFFLE_BTN_CTRL_ID,  &osd_item_height); //shuffle  button
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REVIEW_BTN_CTRL_ID,   &osd_item_height); //review button

        osd_item_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        osd_item_width.add_data = MMIMP3_OSD_BTN_WIDTH;
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REPEAT_BTN_CTRL_ID,   &osd_item_width); //repeat button
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_SHUFFLE_BTN_CTRL_ID,  &osd_item_width); //shuffle  button
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REVIEW_BTN_CTRL_ID,   &osd_item_width); //review button

    }
    else
    {
        GUIFORM_CHILD_WIDTH_T   pda_list_btn_width = {0};
        GUIFORM_CHILD_WIDTH_T   pda_play_btn_width = {0};
        GUIFORM_CHILD_HEIGHT_T  pda_title_main_height = {0};
        GUIFORM_CHILD_HEIGHT_T  pda_title_sub_height = {0};
        GUIFORM_CHILD_HEIGHT_T  osd_item_height = {0};
        GUIFORM_CHILD_WIDTH_T   osd_item_width = {0};
        GUI_RECT_T  file_label_rect = MMIMP3_OSD_LABEL_RECT_V;
        GUI_RECT_T  osd_panel_rect  = MMIMP3_OSD_PANEL_RECT_V;
        GUI_RECT_T  album_anim_rect = MMIMP3_ALBUM_ANIM_RECT_V;
        GUI_RECT_T  ver_title_form_rect = MMIMP3_VER_TITLE_FORM_RECT;
        GUI_RECT_T  ver_play_btn_form_rect = MMIMP3_VER_PLAY_FORM_RECT;
        uint16 button_num = 0;
        uint16 width_button = 0;
        uint16 height_button = 0;
        uint16 button_space = 0;
        uint16 lcd_width = 0;

        //竖屏标题栏
        GUIFORM_IsSlide(MMIMP3_BTN_FORM_CTRL1_ID, FALSE);
        GUIFORM_SetRect(MMIMP3_BTN_FORM_CTRL1_ID, &ver_title_form_rect);
        gui_bg.bg_type = GUI_BG_IMG;
        gui_bg.img_id  = IMAGE_MP3_TITLE_BG;
        GUIFORM_SetBg(MMIMP3_BTN_FORM_CTRL1_ID, &gui_bg);
        GUIFORM_SetAlign(MMIMP3_BTN_FORM_CTRL1_ID, GUIFORM_CHILD_ALIGN_NONE);
        GUIFORM_SetMargin(MMIMP3_BTN_FORM_CTRL1_ID, 0);
        GUIFORM_SetSpace(MMIMP3_BTN_FORM_CTRL2_ID, &hor_space, &ver_space);
        // button元素排版
        pda_list_btn_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        pda_list_btn_width.add_data = MMIMP3_LIST_BTN_WIDTH_V;
#if defined MMI_ISTYLE_SUPPORT 
        if(MMITHEME_IsIstyle())
        {
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID,   &pda_list_btn_width); //return button
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_ISTYLE_OPT_BTN_CTRL_ID,      &pda_list_btn_width); //option button
        }
        else
#endif			
        {
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_LIST_BTN_CTRL_ID,   &pda_list_btn_width); //list button
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL1_ID, MMIMP3_VOLUME_BTN_CTRL_ID, &pda_list_btn_width); //vol  button
        }

        // title元素排版
        GUIFORM_SetMargin(MMIMP3_TITLE_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_TITLE_FORM_CTRL_ID, &hor_space, &ver_space);
        pda_title_main_height.type      = GUIFORM_CHILD_HEIGHT_FIXED;
        pda_title_main_height.add_data  = MMIMP3_TITLE_MAIN_HEIGHT;
        pda_title_sub_height.type       = GUIFORM_CHILD_HEIGHT_FIXED;
        pda_title_sub_height.add_data   = MMIMP3_TITLE_SUB_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_ARTIST_LABLE_CTRL_ID, &pda_title_sub_height);
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_TITLE_LABLE_CTRL_ID,  &pda_title_main_height);
        GUIFORM_SetChildHeight(MMIMP3_TITLE_FORM_CTRL_ID, MMIMP3_ALBUM_LABLE_CTRL_ID,  &pda_title_sub_height);

        //专辑图片区
        GUIANIM_SetCtrlRect(MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID, &album_anim_rect);

        //播放按钮栏
        GUIFORM_IsSlide(MMIMP3_BTN_FORM_FATHER_ID, FALSE);
        ver_play_btn_form_rect.bottom = MMI_MAINSCREEN_HEIGHT - 1 - SOFTKEY_HEIGHT;   //bug2082170
        GUIFORM_SetRect(MMIMP3_BTN_FORM_FATHER_ID, &ver_play_btn_form_rect);
        gui_bg.bg_type = GUI_BG_IMG;
        gui_bg.img_id  = IMAGE_COMMON_BTN_BG;
        GUIFORM_SetBg(MMIMP3_BTN_FORM_FATHER_ID, &gui_bg);
        GUIFORM_SetMargin(MMIMP3_BTN_FORM_FATHER_ID, 0);
        GUIFORM_SetSpace(MMIMP3_BTN_FORM_FATHER_ID, &hor_space, &ver_space);

        GUIRES_GetImgWidthHeight(&width_button, &height_button, IMAGE_COMMON_BTN_PLAY, win_id);
        //        GUIFORM_IsSlide(MMIMP3_BTN_FORM_DUMMY_ID, FALSE);
        //        //GUIFORM_SetBg(MMIMP3_BTN_FORM_DUMMY_ID, &gui_bg);
        //        dummy_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        //        dummy_height.add_data = (ver_play_btn_form_rect.bottom - ver_play_btn_form_rect.top - height_button)/4;
        //        GUIFORM_SetChildHeight(MMIMP3_BTN_FORM_FATHER_ID,MMIMP3_BTN_FORM_DUMMY_ID,&dummy_height);
        //        GUIFORM_SetMargin(MMIMP3_BTN_FORM_DUMMY_ID, 0);
        //        GUIFORM_SetSpace(MMIMP3_BTN_FORM_DUMMY_ID, &hor_space, &ver_space);

        GUIFORM_IsSlide(MMIMP3_BTN_FORM_CTRL2_ID, FALSE);
        GUIFORM_SetBg(MMIMP3_BTN_FORM_CTRL2_ID, &gui_bg);
        GUIFORM_SetMargin(MMIMP3_BTN_FORM_CTRL2_ID, 0);
        GUIFORM_SetAlign(MMIMP3_BTN_FORM_CTRL2_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetSpace(MMIMP3_BTN_FORM_CTRL2_ID, &hor_space, &ver_space);
        // 元素排版
        pda_play_btn_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        /*  if(MMITHEME_IsIstyle())
          {
              pda_play_btn_width.add_data = MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V;
          }
          else
          {
              pda_play_btn_width.add_data = MMIMP3_PLAY_BTN_WIDTH_V;
          }*/
        GUIRES_GetImgWidthHeight(&pda_play_btn_width.add_data, PNULL, IMAGE_COMMON_BTN_PLAY, win_id);
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PREV_BTN_CTRL_ID, &pda_play_btn_width); //pre  button
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PLAY_BTN_CTRL_ID, &pda_play_btn_width); //play button
        GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_NEXT_BTN_CTRL_ID, &pda_play_btn_width); //next button
#if defined MMI_ISTYLE_SUPPORT 
        if(MMITHEME_IsIstyle())
        {
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_LIST_BTN_CTRL_ID,   &pda_play_btn_width); //list  button
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_VOLUME_BTN_CTRL_ID, &pda_play_btn_width); //vol  button
        }
#endif
        {
            uint16  dumy_owndraw_height = ver_play_btn_form_rect.bottom - ver_play_btn_form_rect.top + 1;

            //填充自绘控件确保button垂直居中，后继FORM将支持垂直居中
            pda_play_btn_width.add_data = 1;
            GUIFORM_SetChildWidth(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_BTN_OWNER_DRAW_CTRL_ID, &pda_play_btn_width);
            GUIOWNDRAW_SetHeight(MMIMP3_BTN_OWNER_DRAW_CTRL_ID, (dumy_owndraw_height - dummy_height.add_data));
        }
#if defined MMI_ISTYLE_SUPPORT 
        if(MMITHEME_IsIstyle())
        {
            button_num = 5;
        }
        else
#endif			
        {
            button_num = 3;
        }

        lcd_width = ver_play_btn_form_rect.right - ver_play_btn_form_rect.left + 1;
        button_space = (lcd_width - (width_button * button_num)) / (button_num) - 1;
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PREV_BTN_CTRL_ID, &button_space, PNULL);
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_PLAY_BTN_CTRL_ID, &button_space, PNULL);
        GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_NEXT_BTN_CTRL_ID, &button_space, PNULL);
#if defined MMI_ISTYLE_SUPPORT 
        if(MMITHEME_IsIstyle())
        {
            GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_LIST_BTN_CTRL_ID,   &button_space, PNULL); //list  button
            GUIFORM_SetChildSpace(MMIMP3_BTN_FORM_CTRL2_ID, MMIMP3_VOLUME_BTN_CTRL_ID, &button_space, PNULL); //vol  button
        }
#endif
        //up osd panel
        gui_bg.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
        gui_bg.img_id  = IMAGE_MP3_OSD_BG_UP;
#endif
        GUILABEL_SetRect(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &file_label_rect, FALSE);
        GUILABEL_SetBg(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &gui_bg);

        //down osd panel
        GUIFORM_IsSlide(MMIMP3_OSD_PANEL_FORM_CTRL_ID, FALSE);
        gui_bg.bg_type = GUI_BG_IMG;
        gui_bg.img_id  = IMAGE_MP3_OSD_BG_DOWN;
        GUIFORM_SetBg(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &gui_bg);
        GUIFORM_SetRect(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &osd_panel_rect);
        GUIFORM_SetMargin(MMIMP3_OSD_PANEL_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_OSD_PANEL_FORM_CTRL_ID, &hor_space, &ver_space);

        osd_item_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        osd_item_height.add_data = MMIMP3_OSD_BTN_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_PANEL_FORM_CTRL_ID, MMIMP3_OSD_BTN_FORM_CTRL_ID, &osd_item_height);

        GUIFORM_SetMargin(MMIMP3_PROCESS_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIMP3_PROCESS_FORM_CTRL_ID, &hor_space, &ver_space);
        osd_item_height.add_data = MMIMP3_OSD_PROGRESS_HEIGHT + MMIMP3_OSD_TIME_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_PANEL_FORM_CTRL_ID, MMIMP3_PROCESS_FORM_CTRL_ID, &osd_item_height);
        osd_item_height.add_data = MMIMP3_OSD_PROGRESS_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, &osd_item_height);
        osd_item_height.add_data = MMIMP3_OSD_TIME_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_PROCESS_FORM_CTRL_ID, MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID, &osd_item_height);

        //osd btn
        osd_item_height.add_data =  MMIMP3_OSD_BTN_HEIGHT;
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REPEAT_BTN_CTRL_ID,   &osd_item_height); //repeat button
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_SHUFFLE_BTN_CTRL_ID,  &osd_item_height); //shuffle  button
        GUIFORM_SetChildHeight(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REVIEW_BTN_CTRL_ID,   &osd_item_height); //review button

        osd_item_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
        osd_item_width.add_data = MMIMP3_OSD_BTN_WIDTH;
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REPEAT_BTN_CTRL_ID,   &osd_item_width); //repeat button
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_SHUFFLE_BTN_CTRL_ID,  &osd_item_width); //shuffle  button
        GUIFORM_SetChildWidth(MMIMP3_OSD_BTN_FORM_CTRL_ID, MMIMP3_REVIEW_BTN_CTRL_ID,   &osd_item_width); //review button
    }

    //title label
    GUIFORM_PermitChildFont(MMIMP3_TITLE_FORM_CTRL_ID, FALSE);

    GUILABEL_SetFont(MMIMP3_TITLE_LABLE_CTRL_ID,  MMIMP3_MAIN_TEXT_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetFont(MMIMP3_ARTIST_LABLE_CTRL_ID, MMIMP3_SUB_TEXT_FONT,  MMI_DARK_WHITE_COLOR);
    GUILABEL_SetFont(MMIMP3_ALBUM_LABLE_CTRL_ID,  MMIMP3_SUB_TEXT_FONT,  MMI_DARK_WHITE_COLOR);

    //update album info
    MMIAP_InitAnimCtrlParam();
    MMIAP_UpdateAnimCtrlInfo(FALSE);

    //osd panel
    //MMIAP_CloseMp3OsdPanel(FALSE);
    MMIAP_OpenMp3OsdPanel(FALSE);
}

/*****************************************************************************/
//  Description : SetMp3CtrlParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetMp3CtrlParam(
    MMI_WIN_ID_T win_id
)
{
    //PDA style form param
    SetMp3PdaFormParam(win_id);
    SetMp3BtnParam();
}
