/*****************************************************************************
** File Name:      mmiqbtheme_wintab.c                                        *
** Author:         cheng.luo                                                   *
** Date:           2011/09                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/09/17     cheng.luo       Create.                                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "guilcd.h"
#include "guires.h"
#include "window_parse.h"
#include "mmk_app.h" 
#include "mmi_default.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "guictrl_api.h"
#include "guiform.h"
#include "guilabel.h"
#include "guianim.h"
#include "guilistbox.h"
#include "guibutton.h"
#include "mmitheme_list.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_modu_main.h"
#include "mmipub.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmi_resource.h"
#include "mmifmm_export.h"

#include "mmiqbtheme_export.h"
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_nv.h"
#include "mmiqbtheme_id.h"
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_image.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/      
#define QB_GRAP_HEIGHT			6
#define TWO_LINE_FONT		(MMI_DEFAULT_SMALL_FONT - 2)

#define QB_PANEL_DELAY_TICK			8
#define QB_PANEL_ANIM_FRAME_STEP	8

#define	QB_THEME_INVALID_TP_POINT	(-1)
#define QB_THEME_PREVIEW_MAX_PAGE	3
#ifndef LOW_MEMORY_SUPPORT
#define QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE	4
#else
#define QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE	2
#endif
#define QB_THEME_ICON_PREVIEW_MAX_PAGE		2
#define QB_THEME_ICON_BG_PREVIEW_MAX_PAGE 3
#define QB_THEME_FONT_PREVIEW_MAX_PAGE		2

#if defined MAINLCD_SIZE_320X480
#define QB_THEME_ICON_BG_ICON_ROW		  4
#else
#define QB_THEME_ICON_BG_ICON_ROW		  3
#endif

#if defined MAINLCD_SIZE_320X480
#define QB_THEME_ICON_BG_ICON_COLUMN      4
#else
#define QB_THEME_ICON_BG_ICON_COLUMN	  3
#endif

#define QB_THEME_GLIDE_RATIO		4       //移动距离超过lcd的1/4,则切换
#define QB_THEME_GLIDE_AUTO_FRAME	10		
#define QB_THEME_GLIDE_FRAME_GAP	16

#define QB_THEME_LABEL_MAX_LEN		9

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
enum
{
	QB_INDEX_SYSTEM_LABEL = 0,
	QB_INDEX_SYSTEM_ITEM,
	QB_INDEX_DYNAMIC_LABEL,
	QB_INDEX_FIX_ITEM_MAX,
	QB_INDEX_DYNAMIC_ITEM_START = QB_INDEX_FIX_ITEM_MAX
};

enum
{
	QB_ICON_BG_SYSTEM = 0,
	QB_ICON_BG_TRANSPARENT,
	QB_ICON_BG_THEME,
	QB_ICON_BG_MAX
};

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_qbtheme_is_display_osd = FALSE;
LOCAL BOOLEAN s_qbtheme_has_moved = FALSE;
LOCAL BOOLEAN s_qbtheme_is_tp_panel = FALSE;
LOCAL GUI_POINT_T s_qbtheme_tp_start_pt = {QB_THEME_INVALID_TP_POINT, QB_THEME_INVALID_TP_POINT};
LOCAL GUI_POINT_T s_qbtheme_tp_prev_pt = {QB_THEME_INVALID_TP_POINT, QB_THEME_INVALID_TP_POINT};
LOCAL uint16 s_qbtheme_anim_cur_index = 0;
LOCAL uint16 s_qbtheme_theme_cur_index = 0;
LOCAL BOOLEAN s_qbtheme_is_show_icon = TRUE; 
LOCAL BOOLEAN s_qbtheme_preview_is_readonly = FALSE;

LOCAL GUI_LCD_DEV_INFO s_qbtheme_up_panel_layer = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_qbtheme_down_panel_layer = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_qbtheme_icon_layer = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_qbtheme_icon_bg_layer = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_qbtheme_ret_btn_layer = {0, UILAYER_NULL_HANDLE};

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
/*lint -e785*/
LOCAL CONST MMI_IMAGE_ID_T s_qbtheme_menu_icons[QB_THEME_ICON_BG_ICON_ROW*QB_THEME_ICON_BG_ICON_COLUMN] = 
{
        IMAGE_MAINMENU_ICON_DIAL, IMAGE_MAINMENU_ICON_MESSAGE, IMAGE_MAINMENU_ICON_CL, IMAGE_MAINMENU_ICON_PB,
        IMAGE_MAINMENU_ICON_COUNTEDTIME, IMAGE_MAINMENU_ICON_ENV, IMAGE_MAINMENU_ICON_CALL, IMAGE_MAINMENU_ICON_CAMERA,
        IMAGE_MAINMENU_ICON_SET
#if defined MAINLCD_SIZE_320X480
        , IMAGE_MAINMENU_ICON_AUDIO_PLAYER, 
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
       IMAGE_MAINMENU_ICON_VIDEO_PLAYER,
#endif
#endif
       IMAGE_MAINMENU_ICON_GAME,IMAGE_MAINMENU_ICON_CALENDAR, IMAGE_MAINMENU_ICON_SET, 
#if !defined MMI_RES_SLIM_SUPPORT
#ifdef MMI_MEMO_SUPPORT
       IMAGE_MAINMENU_ICON_NOTE, 
#endif
#endif
#ifdef EBOOK_SUPPORT
        IMAGE_MAINMENU_ICON_EBOOK
#endif
#endif
};
/*lint -e785*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleQbThemeMainWinMsg(
										   MMI_WIN_ID_T      win_id, 	
										   MMI_MESSAGE_ID_E  msg_id,
										   DPARAM            param
										   );

LOCAL MMI_RESULT_E HandleQbThemeListWinMsg(
										   MMI_WIN_ID_T      win_id, 	
										   MMI_MESSAGE_ID_E  msg_id,
										   DPARAM            param
										   );

LOCAL MMI_RESULT_E HandleQbThemeWallpaperListWinMsg(
													MMI_WIN_ID_T      win_id, 	
													MMI_MESSAGE_ID_E  msg_id,
													DPARAM            param
													);

LOCAL MMI_RESULT_E HandleQbThemeIconListWinMsg(
											   MMI_WIN_ID_T      win_id, 	
											   MMI_MESSAGE_ID_E  msg_id,
											   DPARAM            param
											   );

LOCAL MMI_RESULT_E HandleQbThemeIconBgListWinMsg(
												 MMI_WIN_ID_T      win_id, 	
												 MMI_MESSAGE_ID_E  msg_id,
												 DPARAM            param
												 );

LOCAL MMI_RESULT_E HandleQbThemeFontListWinMsg(
												 MMI_WIN_ID_T      win_id, 	
												 MMI_MESSAGE_ID_E  msg_id,
												 DPARAM            param
												 );

LOCAL MMI_RESULT_E HandleQbThemePreviewGlideWinMsg(
												   MMI_WIN_ID_T      win_id, 	
												   MMI_MESSAGE_ID_E  msg_id,
												   DPARAM            param
												   );
LOCAL MMI_RESULT_E HandleQbThemeWallpaperPreviewGlideWinMsg(
															MMI_WIN_ID_T      win_id, 	
															MMI_MESSAGE_ID_E  msg_id,
															DPARAM            param
															);
LOCAL MMI_RESULT_E HandleQbThemeIconPreviewGlideWinMsg(
													   MMI_WIN_ID_T      win_id, 	
													   MMI_MESSAGE_ID_E  msg_id,
													   DPARAM            param
													   );

LOCAL MMI_RESULT_E HandleQbThemeFontPreviewGlideWinMsg(
													   MMI_WIN_ID_T      win_id, 	
													   MMI_MESSAGE_ID_E  msg_id,
													   DPARAM            param
													   );

LOCAL MMI_RESULT_E HandleQbThemeIconBgPreviewGlideWinMsg(
														 MMI_WIN_ID_T      win_id, 	
														 MMI_MESSAGE_ID_E  msg_id,
														 DPARAM            param
														 );

LOCAL void WallpaperSetUpLayerLable(
									MMI_WIN_ID_T win_id, 
									QBTHEME_PACKAGE_INFO_T *info_ptr, 
									uint16 item_max,
									BOOLEAN is_update
									);

/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/
WINDOW_TABLE( QBTHEME_MAIN_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeMainWinMsg),
	WIN_ID(MMIQBTHEME_MAIN_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_THEME_STYLES),

	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, QBTHEME_MAINWIN_FORM_CTRL_ID),
 		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE, QBTHEME_CHANGE_THEME_LABEL_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
  		CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
  			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID, QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID),
  			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, QBTHEME_CHANGE_THEME_AUTHOR_LABEL_CTRL_ID, QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID),
  			CHILD_ANIM_CTRL(FALSE, FALSE, QBTHEME_OVERVIEW_ANIM_CTRL_ID, QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID),

 		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_STYLE_LABEL_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, QBTHEME_CHANGE_STYLE_WALLPAPER_FORM_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL1_CTRL_ID, QBTHEME_CHANGE_STYLE_WALLPAPER_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL2_CTRL_ID, QBTHEME_CHANGE_STYLE_WALLPAPER_FORM_CTRL_ID),
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, QBTHEME_CHANGE_STYLE_ICON_FORM_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, QBTHEME_CHANGE_STYLE_ICON_LABEL1_CTRL_ID, QBTHEME_CHANGE_STYLE_ICON_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_STYLE_ICON_LABEL2_CTRL_ID, QBTHEME_CHANGE_STYLE_ICON_FORM_CTRL_ID),
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, QBTHEME_CHANGE_STYLE_ICONBG_FORM_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, QBTHEME_CHANGE_STYLE_ICONBG_LABEL1_CTRL_ID, QBTHEME_CHANGE_STYLE_ICONBG_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_STYLE_ICONBG_LABEL2_CTRL_ID, QBTHEME_CHANGE_STYLE_ICONBG_FORM_CTRL_ID),

#ifdef QBTHEME_FONT_SUPPORT
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, QBTHEME_CHANGE_STYLE_FONT_FORM_CTRL_ID, QBTHEME_MAINWIN_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, QBTHEME_CHANGE_STYLE_FONT_LABEL1_CTRL_ID, QBTHEME_CHANGE_STYLE_FONT_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, QBTHEME_CHANGE_STYLE_FONT_LABEL2_CTRL_ID, QBTHEME_CHANGE_STYLE_FONT_FORM_CTRL_ID),
#endif
	END_WIN
};

WINDOW_TABLE( QBTHEME_LIST_WIN_TAB ) =
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeListWinMsg),
	WIN_ID(MMIQBTHEME_LIST_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_LOCAL_THEMES),

//	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, QBTHEME_LISTWIN_LISTBOX_CTRL_ID),
#ifdef GUIF_SOFTKEY_PDASTYLE    
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID),
#endif

	END_WIN
};

WINDOW_TABLE( QBTHEME_WALLPAPER_LIST_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeWallpaperListWinMsg),
	WIN_ID(MMIQBTHEME_WALLPAPER_LIST_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_LOCAL_IMAGES),

	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, QBTHEME_WALLPAPER_LISTWIN_LISTBOX_CTRL_ID),
			
	END_WIN
};

WINDOW_TABLE( QBTHEME_ICON_LIST_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeIconListWinMsg),
	WIN_ID(MMIQBTHEME_ICON_LIST_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_LOCAL_ICONS),

	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, QBTHEME_ICON_LISTWIN_LISTBOX_CTRL_ID),
			
	END_WIN
};

WINDOW_TABLE( QBTHEME_FONT_LIST_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeFontListWinMsg),
	WIN_ID(MMIQBTHEME_FONT_LIST_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_LOCAL_FONTS),

	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, QBTHEME_FONT_LISTWIN_LISTBOX_CTRL_ID),
			
	END_WIN
};

WINDOW_TABLE( QBTHEME_ICON_BG_LIST_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeIconBgListWinMsg),
	WIN_ID(MMIQBTHEME_ICON_BG_LIST_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_QB_LOCAL_BORDERS),

	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, QBTHEME_ICON_BG_LISTWIN_LISTBOX_CTRL_ID),
			
	END_WIN
};

WINDOW_TABLE( QBTHEME_PREVIEW_GLIDE_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemePreviewGlideWinMsg),
	WIN_ID(MMIQBTHEME_PREVIEW_GLIDE_WIN_ID),
	WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
	WIN_HIDE_STATUS,
	
	CREATE_ANIM_CTRL(QBTHEME_PREVIEW1_ANIM_CTRL_ID, MMIQBTHEME_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_PREVIEW2_ANIM_CTRL_ID, MMIQBTHEME_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_PREVIEW3_ANIM_CTRL_ID, MMIQBTHEME_PREVIEW_GLIDE_WIN_ID),
	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID),
	
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_LEFT, QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_RIGHT, QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID),

#ifdef GUIF_SOFTKEY_PDASTYLE    
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID),
#endif

	END_WIN
};

WINDOW_TABLE( QBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeWallpaperPreviewGlideWinMsg),
	WIN_ID(MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID),
	WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
	WIN_HIDE_STATUS,
	
	CREATE_ANIM_CTRL(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID, MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID, MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID),
#if QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE > 2
	CREATE_ANIM_CTRL(QBTHEME_WALLPAPER_PREVIEW3_ANIM_CTRL_ID, MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_WALLPAPER_PREVIEW4_ANIM_CTRL_ID, MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID),
#endif
	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID),
	
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_LEFT, QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_RIGHT, QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID),

#ifdef GUIF_SOFTKEY_PDASTYLE
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID),
#endif	
	
	END_WIN
};

WINDOW_TABLE( QBTHEME_ICON_PREVIEW_GLIDE_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeIconPreviewGlideWinMsg),
	WIN_ID(MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID),
	WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
	WIN_HIDE_STATUS,
	
	CREATE_ANIM_CTRL(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID),
	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID),
	
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_LEFT, QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_RIGHT, QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID),

#ifdef GUIF_SOFTKEY_PDASTYLE    
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID),
#endif	
	
	END_WIN
};

WINDOW_TABLE( QBTHEME_FONT_PREVIEW_GLIDE_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeFontPreviewGlideWinMsg),
	WIN_ID(MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID),
	WIN_STYLE(WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
	WIN_HIDE_STATUS,
	
	CREATE_ANIM_CTRL(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID),
	CREATE_ANIM_CTRL(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID),
	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID),
	
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_LEFT, QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_RIGHT, QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID),

#ifdef GUIF_SOFTKEY_PDASTYLE    
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID),
#endif	
	
	END_WIN
};

WINDOW_TABLE( QBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_TAB ) = 
{
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
	WIN_FUNC((uint32)HandleQbThemeIconBgPreviewGlideWinMsg),
	WIN_ID(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID),
	WIN_STYLE(/*WS_HAS_ANIM_BG |*/ WS_DISABLE_COMMON_BG),
	WIN_HIDE_STATUS,
		
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID),
	
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_LEFT, QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_QBTHEME_BTN_RIGHT, QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID),

#ifdef GUIF_SOFTKEY_PDASTYLE    
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID),
	CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_BUTTON, QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID),
#endif	
		
	END_WIN
};

/**--------------------------------------------------------------------------*
**                         EXTERNAL FUNCTION DEFINITION                     *
**--------------------------------------------------------------------------*/
/*******************************************************************/
//  Interface:		ResetPreviewStatus
//  Description : 	ResetPreviewStatus
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetPreviewStatus()
{
	s_qbtheme_has_moved = FALSE;
	s_qbtheme_is_tp_panel = FALSE;
	s_qbtheme_tp_start_pt.x = QB_THEME_INVALID_TP_POINT;
	s_qbtheme_tp_start_pt.y	= QB_THEME_INVALID_TP_POINT;
	s_qbtheme_tp_prev_pt.x = QB_THEME_INVALID_TP_POINT;
	s_qbtheme_tp_prev_pt.y = QB_THEME_INVALID_TP_POINT;
}

/*******************************************************************/
//  Interface:		SetAnimCtrlParam
//  Description : 	SetAnimCtrlParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetAnimCtrlParam(
							MMI_CTRL_ID_T ctrl_id,
							uint32        img_id,
							wchar*		file_ptr,
                            BOOLEAN     is_need_buff,
							BOOLEAN     is_update
							)
{
	GUIANIM_CTRL_INFO_T     control_info = {0};
	GUIANIM_FILE_INFO_T		file_info = {0};
	GUIANIM_DATA_INFO_T		data_info = {0};
	GUIANIM_DISPLAY_INFO_T  display_info = {0};
	
	control_info.is_ctrl_id = TRUE;
	control_info.ctrl_id    = ctrl_id;
	
	display_info.bg.bg_type = GUI_BG_COLOR;
	display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;
	display_info.is_syn_decode = FALSE;
	display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_update      = is_update;
	display_info.is_no_miniatur = TRUE;
	display_info.is_zoom		= FALSE;
	display_info.is_crop_rect	= TRUE;
	
	if (img_id > 0)
	{
		data_info.img_id = 	img_id;	    
        display_info.is_img_res_need_buffer = is_need_buff;

		GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
	}
	else
	{
		file_info.full_path_wstr_ptr = file_ptr;
		file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_ptr);
		
		GUIANIM_SetParam(&control_info, PNULL, &file_info, &display_info);
	}	
}

/*******************************************************************/
//  Interface:		SetButtonGray
//  Description : 	SetButtonGray
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetButtonGray(
						 MMI_CTRL_ID_T ctrl_id,
						 uint32		img_id,
						 uint32		img_gray_id,
						 BOOLEAN	is_gray,  
						 BOOLEAN	is_update 
						 )
{
	GUI_BG_T   bg_btn	= {0};
	
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = (is_gray ? img_gray_id : img_id);	
	
	GUIBUTTON_SetGrayed(ctrl_id, is_gray, &bg_btn, is_update);
}

/*******************************************************************/
//  Interface:		SetButtonFont
//  Description : 	SetButtonFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetButtonFont(
						 MMI_CTRL_ID_T btn_id,
						 CAF_FONT_TYPE_E font	
						 )
{
	GUI_FONT_ALL_T font_btn = {0};
	
	font_btn.font = font;
	font_btn.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);;	
	
	GUIBUTTON_SetFont(btn_id, &font_btn);
}

/*******************************************************************/
//  Interface:		GetRelativeRect
//  Description : 	GetRelativeRect
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL GUI_RECT_T GetRelativeRect(
								 GUI_RECT_T src_rect, 
								 int16 x, 
								 int16 y
								 )
{
	GUI_RECT_T ret_rect = {0};
	
	ret_rect.left = src_rect.left - x;
	ret_rect.right = src_rect.right - x;
	ret_rect.top = src_rect.top - y;
	ret_rect.bottom = src_rect.bottom - y;
	
	return (ret_rect);
}

/*******************************************************************/
//  Interface:		ClearOsdRectBg
//  Description : 	ClearOsdRectBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ClearOsdRectBg(
						  MMI_WIN_ID_T      win_id,
						  GUI_RECT_T		rect
						  )
{
	GUI_RECT_T img_rect = {0};		
	GUI_RECT_T full_rect = UILAYER_GetLayerRect(&s_qbtheme_up_panel_layer);
	
	img_rect = GetRelativeRect(rect, full_rect.left, full_rect.top);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	GUIRES_DisplaySabmImg(&rect, &full_rect, &img_rect, win_id, IMAGE_PDASOFTKEY_BG, &s_qbtheme_up_panel_layer);
#endif
}

/*******************************************************************/
//  Interface:		ResetWallpaperSet
//  Description : 	ResetWallpaperSet
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetWallpaperSet()
{
	MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
	
	MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	if (MMISET_WALLPAPER_FIXED_PIC != wallpaper_info.wallpaper_type)
	{
		wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
		wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
		
		MMIAPISET_SetWallpaperSetInfo(wallpaper_info);			
	}
}

//////////////////////////////////////////////////////////////////////////
//
LOCAL MMI_RESULT_E ReturnQBbtn(void)
{
    MMK_PostMsg(MMK_GerFocusMainWinHandle(), MSG_APP_CANCEL, PNULL, PNULL);
    
    return MMI_RESULT_TRUE;
}

LOCAL void SetRetBtnLayerDisplay(BOOLEAN is_display)
{
    UILAYER_APPEND_BLT_T append_info = {0};
    
    if ((0 != s_qbtheme_ret_btn_layer.block_id) 
        && UILAYER_IsLayerActive(&s_qbtheme_ret_btn_layer))
    {
        append_info.layer_level = UILAYER_LEVEL_HIGH;
        append_info.lcd_dev_info = s_qbtheme_ret_btn_layer;
        
        if (is_display)
        {
            UILAYER_AppendBltLayer(&append_info);		
        }
        else
        {
            UILAYER_RemoveBltLayer(&append_info);
        }
    }
}

LOCAL void DestroyRetBtnLayer(void)
{
    UILAYER_ReleaseLayer(&s_qbtheme_ret_btn_layer);
    s_qbtheme_ret_btn_layer.block_id = UILAYER_NULL_HANDLE;
}

LOCAL void CreateRetBtnLayer(
                             MMI_WIN_ID_T      win_id,
                             GUI_RECT_T        rc_layer
                             )
{
    UILAYER_CREATE_T    create_info = {0};
   	
    if (UILAYER_IsMultiLayerEnable())
    {
        DestroyRetBtnLayer();
        
        //create up layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = rc_layer.left;
        create_info.offset_y = rc_layer.top;
        create_info.width = rc_layer.right - rc_layer.left + 1;
        create_info.height = rc_layer.bottom - rc_layer.top + 1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, &s_qbtheme_ret_btn_layer);
        
        //set color key
        UILAYER_SetLayerColorKey(&s_qbtheme_ret_btn_layer, TRUE,UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&s_qbtheme_ret_btn_layer);
        
        // append
        SetRetBtnLayerDisplay(TRUE);
    }
}

LOCAL void CreateReturnBtn(
                           MMI_WIN_ID_T    win_id,
                           MMI_CTRL_ID_T   ctrl_id 
                           )
{
    GUI_RECT_T rc_ret = QB_PANEL_BACK_BTN_RECT;
    MMI_HANDLE_T btn_ret =  0;
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    
    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.bg.bg_type = GUI_BG_IMG;
    init_data.bg.img_id  = IMAGE_QBTHEME_BTN_RETURN;
    
    btn_ret = GUIBUTTON_CreateDynamic(win_id, ctrl_id, &init_data);
    if (0 != btn_ret)
    {
        GUIBUTTON_SetRect(ctrl_id, &rc_ret);
        GUIBUTTON_SetCallBackFunc(ctrl_id, ReturnQBbtn);
        GUIBUTTON_SetRunSheen(ctrl_id, FALSE);
        
        CreateRetBtnLayer(win_id, rc_ret);
        MMK_SetCtrlLcdDevInfo(ctrl_id, &s_qbtheme_ret_btn_layer);
    }
}

//////////////////////////////////////////////////////////////////////////
// main win
/*******************************************************************/
//  Interface:		GetQbThemeDispName
//  Description : 	GetQbThemeDispName
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN GetQbThemeDispName(
								 QBTHEME_PACKAGE_INFO_T *theme_info_ptr, 
								 MMI_STRING_T  *name_ptr
								 )
{	
	MMISET_LANGUAGE_TYPE_E lang = MMISET_LANGUAGE_SIMP_CHINESE;
	
	if ( PNULL == name_ptr || PNULL == theme_info_ptr)
	{
		return FALSE;
	}	
	
	MMIAPISET_GetLanguageType(&lang);
	//仅在中文且Name_zh不为空下显示中文，其它显示英文
	
	if (MMISET_LANGUAGE_SIMP_CHINESE == lang
		&& 0 != MMIAPICOM_Wstrlen(theme_info_ptr->name_zh))
	{
		name_ptr->wstr_ptr = theme_info_ptr->name_zh;		
	}
	else
	{
		name_ptr->wstr_ptr = theme_info_ptr->name_en;
	}
	name_ptr->wstr_len = (uint16)MMIAPICOM_Wstrlen(name_ptr->wstr_ptr);		
	
	return TRUE;
}

/*******************************************************************/
//  Interface:		QbSetSubTitleLabelParam
//  Description : 	QbSetSubTitleLabelParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QbSetSubTitleLabelParam(
								   MMI_CTRL_ID_T ctrl_id,
								   MMI_TEXT_ID_T text_id
								   )
{
	if (0 != ctrl_id && 0 != text_id)
	{
		GUI_BG_T    label_bg = {0};
		
		label_bg.bg_type = GUI_BG_IMG;
		label_bg.img_id = IMAGE_CONTROL_LIST_SPLIT_ITEM_BG;
		GUILABEL_SetBg(ctrl_id, &label_bg);
		GUILABEL_SetTextById(ctrl_id, text_id, FALSE);
		GUILABEL_SetFont(ctrl_id, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR); 
	}
}

/*******************************************************************/
//  Interface:		QbInitFormParam
//  Description : 	QbInitFormParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QbInitFormParam(
						   MMI_CTRL_ID_T form_ctrl, 
						   uint16 margin
						   )
{
	GUIFORM_SetStyle(form_ctrl, GUIFORM_STYLE_UNIT);	
	GUIFORM_SetMargin(form_ctrl, margin);
}

/*******************************************************************/
//  Interface:		QbSetNameLableParam
//  Description : 	QbSetNameLableParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QbSetNameLableParam(
							   MMI_CTRL_ID_T name_ctrl,
							   MMI_TEXT_ID_T name_id,
							   wchar *name_zh_ptr,
							   wchar *name_en_ptr
							   )
{
	MMI_STRING_T  str_name = {0};
	MMISET_LANGUAGE_TYPE_E lang = MMISET_LANGUAGE_SIMP_CHINESE;
	
	GUILABEL_SetFont(name_ctrl, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	
	if (0 < name_id)
	{
		GUILABEL_SetTextById(name_ctrl, name_id, FALSE);
	}
	else
	{
		MMIAPISET_GetLanguageType(&lang);
		//仅在中文且Name_zh不为空下显示中文，其它显示英文	
		if (MMISET_LANGUAGE_SIMP_CHINESE == lang
			&& 0 != MMIAPICOM_Wstrlen(name_zh_ptr))
		{
			str_name.wstr_ptr = name_zh_ptr;
		}
		else
		{
			str_name.wstr_ptr = name_en_ptr;
		}
		
		if (PNULL != str_name.wstr_ptr)
		{
			str_name.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_name.wstr_ptr);
			
			GUILABEL_SetText(name_ctrl, &str_name, FALSE);
		}
	}
}

/*******************************************************************/
//  Interface:		QbSetAuthorLableParam
//  Description : 	QbSetAuthorLableParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QbSetAuthorLableParam(
								 MMI_CTRL_ID_T author_ctrl,
							     MMI_TEXT_ID_T author_id,
								 wchar* author_ptr
								 )
{
	MMI_STRING_T  str_author = {0};
	
	GUILABEL_SetFont(author_ctrl, TWO_LINE_FONT, MMI_DARK_GRAY_COLOR);
	
	if (0 < author_id)
	{
		GUILABEL_SetTextById(author_ctrl, author_id, FALSE);
	}
	else if (PNULL != author_ptr)
	{
		str_author.wstr_ptr = author_ptr;
		str_author.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_author.wstr_ptr);
		GUILABEL_SetText(author_ctrl, &str_author, FALSE);	
	}
}	

/*******************************************************************/
//  Interface:		SetWallpaperChildForm
//  Description : 	SetWallpaperChildForm
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetWallpaperChildForm(
										 QBTHEME_PACKAGE_INFO_T *info_ptr, 
										 wchar* wallpaper_file_ptr
										 )
{
	MMI_CTRL_ID_T   ctrl_id = QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL2_CTRL_ID;
	MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
	
	MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	if (MMISET_WALLPAPER_FIXED_PIC != wallpaper_info.wallpaper_type)
	{
		GUILABEL_SetTextById(ctrl_id, TXT_EXPORT_LOC_CUSTOM, FALSE);
	}	
	else if (0 == MMIAPICOM_Wstrlen(wallpaper_file_ptr) || PNULL == info_ptr)
	{
		GUILABEL_SetTextById(ctrl_id, TXT_COMMON_CLASSIC, FALSE);
	}
	else
	{
		MMI_STRING_T  str_wallpaper = {0};
		
		GetQbThemeDispName(info_ptr, &str_wallpaper);
		GUILABEL_SetText(ctrl_id, &str_wallpaper, FALSE);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetIconChildForm
//  Description : 	SetIconChildForm
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetIconChildForm(
									QBTHEME_PACKAGE_INFO_T *info_ptr, 
									uint8 type
									)
{
	MMI_CTRL_ID_T   ctrl_id = QBTHEME_CHANGE_STYLE_ICON_LABEL2_CTRL_ID;
	
	if (SYSTEM_DEFAULT == type || PNULL == info_ptr)
	{
		GUILABEL_SetTextById(ctrl_id, TXT_COMMON_CLASSIC, FALSE);
	}
	else if (THEME_DEFAULT == type)
	{
		MMI_STRING_T  str_icon = {0};
		
		GetQbThemeDispName(info_ptr, &str_icon);
		GUILABEL_SetText(ctrl_id, &str_icon, FALSE);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetFontChildForm
//  Description : 	SetFontChildForm
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetFontChildForm(
									QBTHEME_PACKAGE_INFO_T *info_ptr, 
									uint8 type
									)
{
	MMI_CTRL_ID_T   ctrl_id = QBTHEME_CHANGE_STYLE_FONT_LABEL2_CTRL_ID;
	
	if (SYSTEM_DEFAULT == type || PNULL == info_ptr 
		|| 0 == MMIQBTHEME_GetPkgFontSize(info_ptr))
	{
		GUILABEL_SetTextById(ctrl_id, TXT_COMMON_CLASSIC, FALSE);
	}
	else if (THEME_DEFAULT == type)
	{
		MMI_STRING_T  str_icon = {0};
		
		GetQbThemeDispName(info_ptr, &str_icon);
		GUILABEL_SetText(ctrl_id, &str_icon, FALSE);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetIconBgChildForm
//  Description : 	SetIconBgChildForm
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetIconBgChildForm(
									  QBTHEME_PACKAGE_INFO_T *info_ptr, 
									  uint8 type
									  )
{
	MMI_CTRL_ID_T   ctrl_id = QBTHEME_CHANGE_STYLE_ICONBG_LABEL2_CTRL_ID;
	
	if (DEFAULT_NONE == type)
	{
		GUILABEL_SetTextById(ctrl_id, TXT_QB_TRANSPARENT, FALSE);
	}
	else if (SYSTEM_DEFAULT == type || PNULL == info_ptr)
	{
		GUILABEL_SetTextById(ctrl_id, TXT_COMMON_CLASSIC, FALSE);
	}
	else if (THEME_DEFAULT == type)
	{
		MMI_STRING_T  str_iconbg = {0};
		
		GetQbThemeDispName(info_ptr, &str_iconbg);
		GUILABEL_SetText(ctrl_id, &str_iconbg, FALSE);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		InitComponentItem
//  Description : 	InitComponentItem
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void InitComponentItem(
							 MMI_HANDLE_T form_id, 
							 MMI_HANDLE_T label1, 
							 MMI_TEXT_ID_T component, 
							 MMI_HANDLE_T label2
							 )
{
	GUIFORM_SetStyle(form_id, GUIFORM_STYLE_UNIT);	
	GUIFORM_SetMargin(form_id, QB_FORM_MARGIN);
	GUILABEL_SetFont(label1,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	GUILABEL_SetFont(label2, TWO_LINE_FONT, MMI_DARK_GRAY_COLOR);
	GUILABEL_SetTextById(label1, component, FALSE);	
}

/*******************************************************************/
//  Interface:		QBThemeMainFormParamInit
//  Description : 	QBThemeMainFormParamInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeMainFormParamInit(void)
{
    uint16		overview_h = 0;
	uint16      grap_space = QB_GRAP_HEIGHT;
	GUIFORM_CHILD_HEIGHT_T  anim_height = {0};
	
	// main form
	GUIFORM_SetType(QBTHEME_MAINWIN_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIFORM_SetMargin(QBTHEME_MAINWIN_FORM_CTRL_ID, 0);
	GUIFORM_PermitChildBg(QBTHEME_MAINWIN_FORM_CTRL_ID, FALSE);
	GUIFORM_PermitChildFont(QBTHEME_MAINWIN_FORM_CTRL_ID, FALSE);
	
	// theme label
	QbSetSubTitleLabelParam(QBTHEME_CHANGE_THEME_LABEL_CTRL_ID, TXT_QB_CURRENT_THEME/*TXT_QB_CHANGE_THEMES*/);
	
	// change theme form
	QbInitFormParam(QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID, QB_FORM_MARGIN);
	
	GUILABEL_SetFont(QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	GUILABEL_SetFont(QBTHEME_CHANGE_THEME_AUTHOR_LABEL_CTRL_ID, TWO_LINE_FONT, MMI_DARK_GRAY_COLOR);
	
	GUIRES_GetImgWidthHeight(PNULL, &overview_h, IMAGE_THEME_OVERVIEW, MMIQBTHEME_MAIN_WIN_ID);
	anim_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
	anim_height.add_data = overview_h;
	GUIFORM_SetChildHeight(QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID, QBTHEME_OVERVIEW_ANIM_CTRL_ID, &anim_height);

	//grab line
	GUIFORM_SetChildSpace(QBTHEME_MAINWIN_FORM_CTRL_ID, QBTHEME_CHANGE_STYLE_LABEL_CTRL_ID, PNULL, &grap_space);
	
	// style label
	QbSetSubTitleLabelParam(QBTHEME_CHANGE_STYLE_LABEL_CTRL_ID, TXT_QB_CHANGE_COMPONENTS);
	
	// wallpaper form
	InitComponentItem(QBTHEME_CHANGE_STYLE_WALLPAPER_FORM_CTRL_ID, QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL1_CTRL_ID,\
		TXT_QB_WALLPAPER, QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL2_CTRL_ID);
	
	// icon form
	InitComponentItem(QBTHEME_CHANGE_STYLE_ICON_FORM_CTRL_ID, QBTHEME_CHANGE_STYLE_ICON_LABEL1_CTRL_ID,\
		TXT_QB_THEME_ICONS, QBTHEME_CHANGE_STYLE_ICON_LABEL2_CTRL_ID);
	
	// icon bg form
	InitComponentItem(QBTHEME_CHANGE_STYLE_ICONBG_FORM_CTRL_ID, QBTHEME_CHANGE_STYLE_ICONBG_LABEL1_CTRL_ID,\
		TXT_QB_THEME_BORDERS, QBTHEME_CHANGE_STYLE_ICONBG_LABEL2_CTRL_ID);

	// font form
	InitComponentItem(QBTHEME_CHANGE_STYLE_FONT_FORM_CTRL_ID, QBTHEME_CHANGE_STYLE_FONT_LABEL1_CTRL_ID,\
		TXT_QB_THEME_FONTS, QBTHEME_CHANGE_STYLE_FONT_LABEL2_CTRL_ID);

	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetChangeThemeChildForm
//  Description : 	SetChangeThemeChildForm
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetChangeThemeChildForm(
										   QBTHEME_PACKAGE_INFO_T *theme_ptr
										   )
{
	// system default
	if (PNULL == theme_ptr) 
	{
		GUILABEL_SetTextById(QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID, TXT_COMMON_CLASSIC, FALSE);
		GUILABEL_SetTextById(QBTHEME_CHANGE_THEME_AUTHOR_LABEL_CTRL_ID, TXT_QB_SYSTEM_DEFAULT, FALSE);		
		
		SetAnimCtrlParam(QBTHEME_OVERVIEW_ANIM_CTRL_ID, IMAGE_THEME_OVERVIEW, PNULL, FALSE, FALSE);
	}
	else
	{	
		wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		
		QbSetNameLableParam(QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID, 0, theme_ptr->name_zh, theme_ptr->name_en);		
		QbSetAuthorLableParam(QBTHEME_CHANGE_THEME_AUTHOR_LABEL_CTRL_ID, 0, theme_ptr->author);
		
		MMIQBTHEME_GetImagePath(theme_ptr, QB_IMG_OVERVIEW, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		SetAnimCtrlParam(QBTHEME_OVERVIEW_ANIM_CTRL_ID, 0, anim_file, FALSE, FALSE);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetQBThemeMainFormParam
//  Description : 	SetQBThemeMainFormParam
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetQBThemeMainFormParam()
{	
	QBTHEME_PACKAGE_INFO_T *info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	// set child form by theme
	SetChangeThemeChildForm(info_ptr);
	
	// set wallpaper
	SetWallpaperChildForm(info_ptr, MMIQBTHEME_GetCurWallpaper());
	
	// set icon
	SetIconChildForm(info_ptr, MMIQBTHEME_GetCurIconType());
	
	// set icon bg	
	SetIconBgChildForm(info_ptr, MMIQBTHEME_GetCurIconBgType());
	
	// set font
	SetFontChildForm(info_ptr, MMIQBTHEME_GetCurFontType());
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleQbThemeMainWinMsg
//  Description : 	HandleQbThemeMainWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeMainWinMsg(
										   MMI_WIN_ID_T      win_id, 	
										   MMI_MESSAGE_ID_E  msg_id,
										   DPARAM            param
										   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_CHANGE_THEME_CONTENT_FORM_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		QBThemeMainFormParamInit();
		SetQBThemeMainFormParam();
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
        if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
        switch (ctrl_id)
		{
		case QBTHEME_CHANGE_THEME_LABEL_CTRL_ID:
		case QBTHEME_CHANGE_STYLE_LABEL_CTRL_ID:
			break;
		case QBTHEME_CHANGE_THEME_NAME_LABEL_CTRL_ID:
            {
                uint16 cur_idx = MMIQBTHEME_GetIndexByName(MMIQBTHEME_GetCurrentTheme()) + 1; // add system
                s_qbtheme_preview_is_readonly = TRUE;
                MMK_CreateWin((uint32 *)QBTHEME_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)cur_idx);
            }
			break;
		case QBTHEME_CHANGE_STYLE_WALLPAPER_LABEL2_CTRL_ID:
			MMK_CreateWin((uint32 *)QBTHEME_WALLPAPER_LIST_WIN_TAB, PNULL);
			break;
		case QBTHEME_CHANGE_STYLE_ICON_LABEL2_CTRL_ID:
			MMK_CreateWin((uint32 *)QBTHEME_ICON_LIST_WIN_TAB, PNULL);
			break;
		case QBTHEME_CHANGE_STYLE_ICONBG_LABEL2_CTRL_ID:
			MMK_CreateWin((uint32 *)QBTHEME_ICON_BG_LIST_WIN_TAB, PNULL);
			break;
		case QBTHEME_CHANGE_STYLE_FONT_LABEL2_CTRL_ID:
			MMK_CreateWin((uint32 *)QBTHEME_FONT_LIST_WIN_TAB, PNULL);
			break;
		default:
			break;
		}
		break;
		
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;
		case MSG_CLOSE_WINDOW:
			break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// themem list win

/*******************************************************************/
//  Interface:		OpenExplorerWin
//  Description : 	OpenExplorerWin
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E OpenExplorerWin(void)
{
    MMIFMM_OPENWIN_INFO_T openwin_info = {0};
    
    openwin_info.path_ptr = PNULL;
    openwin_info.path_len = 0;
    openwin_info.filename = PNULL;
    openwin_info.file_len = 0;
    openwin_info.is_need_display = FALSE;
    openwin_info.find_suffix_type = MMIFMM_THEME_QB;
#ifdef DRM_SUPPORT
    openwin_info.find_suffix_type |= MMIFMM_DRM_ALL;
#endif
    openwin_info.sk_text_id.leftsoft_id = TXT_NULL;
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;//返回
    openwin_info.callback.callback_ok = PNULL;
    openwin_info.callback.callback_web = PNULL;
    openwin_info.callback.callback_can = PNULL;
    
    MMIAPIFMM_OpenExplorerWin(&openwin_info);
    
    return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ChangeQBComponents
//  Description : 	ChangeQBComponents
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ChangeQBComponents(void)
{
    MMK_CreateWin((uint32 *)QBTHEME_MAIN_WIN_TAB, PNULL);
    
    return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ThemeListInit
//  Description : 	ThemeListInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ThemeListInit(
                        MMI_WIN_ID_T      win_id
                         )
{
    GUI_BG_T   bg_btn	= {0};
    GUI_RECT_T rc_key_l = QB_PANEL_LEFT_KEY_RECT;
    GUI_RECT_T rc_key_r = QB_PANEL_RIGHT_KEY_RECT;
    GUI_BOTH_RECT_T rc_client = MMITHEME_GetWinClientBothRect(win_id);
    
    // L button
    GUIBUTTON_SetRect(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, &rc_key_l);
#ifdef GUIF_SOFTKEY_PDASTYLE    
    bg_btn.bg_type = GUI_BG_IMG;
    bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
    GUIBUTTON_SetPressedBg(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, &bg_btn);
#endif    
    SetButtonFont(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
    GUIBUTTON_SetTextId(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, TXT_QB_CHANGE_COMPONENTS);
    GUIBUTTON_SetCallBackFunc(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, ChangeQBComponents);
    GUIBUTTON_SetRunSheen(QBTHEME_LISTWIN_CHANGE_BTN_CTRL_ID, FALSE);
    
    // R button
    GUIBUTTON_SetRect(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, &rc_key_r);
#ifdef GUIF_SOFTKEY_PDASTYLE    
    bg_btn.bg_type = GUI_BG_IMG;
    bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
    GUIBUTTON_SetPressedBg(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, &bg_btn);
#endif    
    SetButtonFont(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
    GUIBUTTON_SetTextId(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, TXT_QB_LOCAL_THEME_PACK);
    GUIBUTTON_SetCallBackFunc(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, OpenExplorerWin);
    GUIBUTTON_SetRunSheen(QBTHEME_LISTWIN_EXPLORER_BTN_CTRL_ID, FALSE);
    
    // list
    {
        GUILIST_INIT_DATA_T     list_init = {0};
        
        list_init.both_rect = rc_client;
        list_init.both_rect.v_rect.bottom = rc_client.v_rect.bottom - QB_PANEL_LAYER_HEIGHT;        
        list_init.type = GUILIST_TEXTLIST_E;
        
        GUILIST_CreateListBox(win_id, 0, QBTHEME_LISTWIN_LISTBOX_CTRL_ID, &list_init);
    }
}

/*******************************************************************/
//  Interface:		AppendListThemeItem
//  Description : 	AppendListThemeItem
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void AppendListThemeItem(
							   MMI_CTRL_ID_T    ctrl_id,
							   GUIITEM_STYLE_E  style
							   )
{
	GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
	
	item_t.item_style = style;
	if (GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT == item_t.item_style)
	{
		item_t.item_state |=GUIITEM_STATE_SPLIT;
	}
	
    item_t.item_data_ptr = &item_data;
	
	GUILIST_AppendItem(ctrl_id, &item_t);
}

/*******************************************************************/
//  Interface:		GetListDynamicItemContent
//  Description : 	GetListDynamicItemContent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetListDynamicItemContent(
									 MMI_CTRL_ID_T ctrl_id,
									 QBTHEME_IMAGE_ID_E img_id,
									 QBTHEME_PACKAGE_INFO_T *info_ptr,
									 GUILIST_NEED_ITEM_CONTENT_T *content_ptr
									 )
{
	GUIITEM_CONTENT_T   item_content = {0};
	GUIANIM_FILE_INFO_T file_info = {0};
	wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};	
	
	if (PNULL == content_ptr || PNULL == info_ptr)
	{
		return;
	}
	
	MMIQBTHEME_GetImagePath(info_ptr, img_id, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
	
	file_info.full_path_wstr_ptr = anim_file;
	file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_info.full_path_wstr_ptr);
	
	item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
	item_content.item_data.anim_path_ptr = &file_info;
	
	GUILIST_SetItemContent(ctrl_id, &item_content, content_ptr->item_index, content_ptr->item_content_index);
}

/*******************************************************************/
//  Interface:		QBThemeListboxInit
//  Description : 	QBThemeListboxInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeListboxInit(
									  MMI_CTRL_ID_T ctrl_id
									  )
{
	uint32		item_count = 0;
	uint32      i = 0;
	
	item_count = MMIQBTHEME_GetDownloadCount();
	
	if (0 == item_count)
	{
		item_count = QB_INDEX_FIX_ITEM_MAX - 1; 
	}
	else
	{
		item_count += QB_INDEX_FIX_ITEM_MAX;
	}
	
	GUILIST_RemoveAllItems(ctrl_id);
	GUILIST_SetMaxItem(ctrl_id, (uint16)item_count, TRUE);
	
	// 0, 1
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM);
	
	if (item_count > QB_INDEX_FIX_ITEM_MAX)
	{
		// 2
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
		
		// 3...
		for (i = QB_INDEX_FIX_ITEM_MAX; i < item_count; i++)
		{
			AppendListThemeItem(ctrl_id, GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM);	
		}
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		GetThemeListDynamicItemData
//  Description : 	GetThemeListDynamicItemData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetThemeListDynamicItemData(
									   QBTHEME_PACKAGE_INFO_T* info_ptr,
									   GUILIST_ITEM_DATA_T* data_ptr
									   )
{
	//QBTHEME_PACKAGE_INFO_T *info_ptr = MMIQBTHEME_GetInfoByIndex(index);
	MMI_STRING_T  str_name = {0};
	MMI_STRING_T  str_author = {0};
	
	if (PNULL == data_ptr || PNULL == info_ptr)
	{
		return;
	}
	
	// name
	GetQbThemeDispName(info_ptr, &str_name);
	data_ptr->item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	data_ptr->item_content[0].item_data.text_buffer = str_name;
	
	// author
	str_author.wstr_ptr = info_ptr->author;
	str_author.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_author.wstr_ptr);
	data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	data_ptr->item_content[1].item_data.text_buffer = str_author;
	
	// overview
	data_ptr->item_content[2].item_data_type = GUIITEM_DATA_ANIM_PATH;
	
	// mark
	data_ptr->item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	if (0 == MMIAPICOM_Wstrcmp(info_ptr->name_en, MMIQBTHEME_GetCurrentTheme()))
	{
		data_ptr->item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
	}
	else
	{
		data_ptr->item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;
	}	 
}

/*******************************************************************/
//  Interface:		HandleThemeListNeedData
//  Description : 	HandleThemeListNeedData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleThemeListNeedData(
								   MMI_CTRL_ID_T		ctrl_id, 
								   GUILIST_NEED_ITEM_DATA_T *data_ptr
								   )
{
	uint16				cur_index = 0;
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
	
	if (PNULL != data_ptr)
	{
		cur_index = data_ptr->item_index;	
		switch(cur_index)
		{
		case QB_INDEX_SYSTEM_LABEL:
			{
				item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
				item_data.item_content[0].item_data.text_id = TXT_QB_SYSTEM_DEFAULT;
			}
			break;
		case QB_INDEX_SYSTEM_ITEM:
			{
				item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
				item_data.item_content[0].item_data.text_id = TXT_COMMON_CLASSIC;
				
				item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
				item_data.item_content[1].item_data.text_id = TXT_QB_SYSTEM_DEFAULT;
				
				item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
				item_data.item_content[2].item_data.image_id = IMAGE_THEME_OVERVIEW;
				
				item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
				if (0 == MMIAPICOM_Wstrlen(MMIQBTHEME_GetCurrentTheme()))
				{
					item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
				}
				else
				{
					item_data.item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;
				}
			}
			break;
		case QB_INDEX_DYNAMIC_LABEL:
			{
				item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
				item_data.item_content[0].item_data.text_id = TXT_QB_DOWNLOADS;
			}
			break;
		default:
			{
				info_ptr = MMIQBTHEME_GetInfoByIndex(cur_index - QB_INDEX_DYNAMIC_ITEM_START);
				GetThemeListDynamicItemData(info_ptr, &item_data);
			}
			break;
		}
		
		GUILIST_SetItemData(ctrl_id, &item_data, cur_index);
	}	
}

/*******************************************************************/
//  Interface:		HandleThemeListNeedContent
//  Description : 	HandleThemeListNeedContent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleThemeListNeedContent(
									  MMI_CTRL_ID_T			ctrl_id, 
									  GUILIST_NEED_ITEM_CONTENT_T *content_ptr
									  )
{	
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
	
	if (PNULL != content_ptr)
	{
		info_ptr = MMIQBTHEME_GetInfoByIndex(content_ptr->item_index - QB_INDEX_DYNAMIC_ITEM_START);	
		GetListDynamicItemContent(ctrl_id, QB_IMG_OVERVIEW, info_ptr, content_ptr);
	}	
}

/*******************************************************************/
//  Interface:		HandleQbThemeListWinMsg
//  Description : 	HandleQbThemeListWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeListWinMsg(
										   MMI_WIN_ID_T      win_id, 	
										   MMI_MESSAGE_ID_E  msg_id,
										   DPARAM            param
										   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_LISTWIN_LISTBOX_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
        ThemeListInit(win_id);
		MMK_SendMsg(win_id, MSG_UPDATE_QBTHEME_LIST, 0);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_UPDATE_QBTHEME_LIST:
		QBThemeListboxInit(ctrl_id);
		break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		HandleThemeListNeedData(ctrl_id, (GUILIST_NEED_ITEM_DATA_T *)param);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		HandleThemeListNeedContent(ctrl_id, (GUILIST_NEED_ITEM_CONTENT_T *)param);
		break;
	case MSG_GET_FOCUS:
        MMK_SendMsg(win_id, MSG_UPDATE_QBTHEME_LIST, 0);
		break;
	case MSG_FULL_PAINT:
        {
#ifdef GUIF_SOFTKEY_PDASTYLE
            uint16              lcd_width = 0;
            uint16              lcd_height = 0;
            GUI_RECT_T          rc_btn_bg = {0};
            
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            rc_btn_bg.left = 0;
            rc_btn_bg.right = lcd_width - 1;
            rc_btn_bg.bottom = lcd_height - 1;
            rc_btn_bg.top = rc_btn_bg.bottom - QB_PANEL_LAYER_HEIGHT;
            
            GUIRES_DisplayImg(PNULL, &rc_btn_bg, PNULL, win_id, IMAGE_PDASOFTKEY_BG, MMITHEME_GetDefaultLcdDev());
#endif
        }
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
		{
			uint16 list_index = GUILIST_GetCurItemIndex(ctrl_id );
			
			if (QB_INDEX_SYSTEM_ITEM == list_index)
			{
				list_index = 0;
			}
			else
			{
				list_index -= QB_INDEX_DYNAMIC_LABEL;
			}
			
			MMK_CreateWin((uint32 *)QBTHEME_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)list_index);
		}
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// icon list win
/*******************************************************************/
//  Interface:		GetListSplitItemDate
//  Description : 	GetListSplitItemDate
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetListSplitItemDate(
								MMI_TEXT_ID_T text_id,
								GUILIST_ITEM_DATA_T* data_ptr
								)
{
	if (PNULL != data_ptr)
	{
		data_ptr->item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
		data_ptr->item_content[0].item_data.text_id = text_id;
	}
}

/*******************************************************************/
//  Interface:		GetListDefaulItemtData
//  Description : 	GetListDefaulItemtData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetListDefaulItemtData(
								  MMI_IMAGE_ID_T	img_id,
								  MMI_TEXT_ID_T		text_id,
								  GUILIST_ITEM_DATA_T* data_ptr
								  )
{
	if (PNULL != data_ptr)
	{
		data_ptr->item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
		data_ptr->item_content[0].item_data.image_id = img_id;
		data_ptr->item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
		data_ptr->item_content[1].item_data.text_id = text_id;
		data_ptr->item_content[2].item_data_type    = GUIITEM_DATA_IMAGE_ID;
		data_ptr->item_content[2].item_data.image_id = IMAGE_COMMON_RIGHT_ARROW;
	}
}

/*******************************************************************/
//  Interface:		GetListDynamicItemData
//  Description : 	GetListDynamicItemData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void GetListDynamicItemData(
								  MMI_TEXT_ID_T name_id,
								  wchar*		name_ptr,
								  GUILIST_ITEM_DATA_T* data_ptr
								  )
{
	MMI_STRING_T  str_name = {0};
	
	if (PNULL != data_ptr)
	{
		data_ptr->item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
		
		if (0 < name_id)
		{
			data_ptr->item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
			data_ptr->item_content[1].item_data.text_id = name_id;
		}
		else
		{
			str_name.wstr_ptr = name_ptr;
			str_name.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_name.wstr_ptr);
			data_ptr->item_content[1].item_data_type        = GUIITEM_DATA_TEXT_BUFFER;
			data_ptr->item_content[1].item_data.text_buffer = str_name;
		}
		
		data_ptr->item_content[2].item_data_type    = GUIITEM_DATA_IMAGE_ID;
		data_ptr->item_content[2].item_data.image_id = IMAGE_COMMON_RIGHT_ARROW;
	}	
}

LOCAL void HandleIconListNeedData(
								   MMI_CTRL_ID_T ctrl_id, 
								   GUILIST_NEED_ITEM_DATA_T *data_ptr
								   )
{
	uint16				cur_index = 0;
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	
	item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;
	
	if (PNULL != data_ptr)
	{
		cur_index = data_ptr->item_index;	
		switch(cur_index)
		{
		case 0:
			GetListDefaulItemtData(IMAGE_THEME_ICON_PREVIEW, TXT_COMMON_CLASSIC, &item_data);
			if (SYSTEM_DEFAULT == MMIQBTHEME_GetCurIconType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		case 1:
			GetListDynamicItemData(TXT_QB_THEME_DEFAULT, PNULL, &item_data);
			if (THEME_DEFAULT == MMIQBTHEME_GetCurIconType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		default:
			break;
		}
		
		GUILIST_SetItemData(ctrl_id, &item_data, cur_index);
	}	
}

/*******************************************************************/
//  Interface:		QBThemeIconListboxInit
//  Description : 	QBThemeIconListboxInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeIconListboxInit(
										  MMI_CTRL_ID_T ctrl_id
										  )
{
	uint32		i = 0;
	uint16		item_count = QB_THEME_ICON_PREVIEW_MAX_PAGE;
	
	if (PNULL == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	GUILIST_SetMaxItem(ctrl_id, item_count, TRUE);
	
	for (i = 0; i < item_count; i++)
	{
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleQbThemeIconListWinMsg
//  Description : 	HandleQbThemeIconListWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeIconListWinMsg(
											   MMI_WIN_ID_T      win_id, 	
											   MMI_MESSAGE_ID_E  msg_id,
											   DPARAM            param
											   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_ICON_LISTWIN_LISTBOX_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		QBThemeIconListboxInit(ctrl_id);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		HandleIconListNeedData(ctrl_id, (GUILIST_NEED_ITEM_DATA_T *)param);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		GetListDynamicItemContent(ctrl_id, QB_IMG_ICON_PRIVIEW, MMIQBTHEME_GetCurThemeInfo(), (GUILIST_NEED_ITEM_CONTENT_T *)param);
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
		{
			uint16 list_index = GUILIST_GetCurItemIndex(ctrl_id );		
			MMK_CreateWin((uint32 *)QBTHEME_ICON_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)list_index);
		}	
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// font list win
/*******************************************************************/
//  Interface:		HandleFontListNeedData
//  Description : 	HandleFontListNeedData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleFontListNeedData(
								   MMI_CTRL_ID_T ctrl_id, 
								   GUILIST_NEED_ITEM_DATA_T *data_ptr
								   )
{
	uint16				cur_index = 0;
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	
	item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;
	
	if (PNULL != data_ptr)
	{
		cur_index = data_ptr->item_index;	
		switch(cur_index)
		{
		case 0:
			GetListDefaulItemtData(IMAGE_THEME_FONT_PREVIEW, TXT_COMMON_CLASSIC, &item_data);
			if (SYSTEM_DEFAULT == MMIQBTHEME_GetCurFontType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		case 1:
			GetListDynamicItemData(TXT_QB_THEME_DEFAULT, PNULL, &item_data);
			if (THEME_DEFAULT == MMIQBTHEME_GetCurFontType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		default:
			break;
		}
		
		GUILIST_SetItemData(ctrl_id, &item_data, cur_index);
	}	
}

/*******************************************************************/
//  Interface:		QBThemeFontListboxInit
//  Description : 	QBThemeFontListboxInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeFontListboxInit(
										  MMI_CTRL_ID_T ctrl_id
										  )
{
	uint32		i = 0;
	uint16		item_count = QB_THEME_FONT_PREVIEW_MAX_PAGE;
	QBTHEME_PACKAGE_INFO_T* info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	// if font lib not exist, -1
	if (PNULL == info_ptr 
		|| 0 == MMIQBTHEME_GetPkgFontSize(info_ptr))
	{
		item_count -= 1;
	}
	
	GUILIST_SetMaxItem(ctrl_id, item_count, TRUE);
	
	for (i = 0; i < item_count; i++)
	{
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleQbThemeFontListWinMsg
//  Description : 	HandleQbThemeFontListWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeFontListWinMsg(
											   MMI_WIN_ID_T      win_id, 	
											   MMI_MESSAGE_ID_E  msg_id,
											   DPARAM            param
											   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_FONT_LISTWIN_LISTBOX_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		QBThemeFontListboxInit(ctrl_id);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		HandleFontListNeedData(ctrl_id, (GUILIST_NEED_ITEM_DATA_T *)param);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		GetListDynamicItemContent(ctrl_id, QB_IMG_FONT_PREVIEW, MMIQBTHEME_GetCurThemeInfo(), (GUILIST_NEED_ITEM_CONTENT_T *)param);
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
		{
			uint16 list_index = GUILIST_GetCurItemIndex(ctrl_id );		
			
			MMK_CreateWin((uint32 *)QBTHEME_FONT_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)list_index);
		}	
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// icon bg list win
/*******************************************************************/
//  Interface:		HandleIconBgListNeedData
//  Description : 	HandleIconBgListNeedData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleIconBgListNeedData(
									MMI_CTRL_ID_T	ctrl_id, 
									GUILIST_NEED_ITEM_DATA_T *data_ptr
									)
{
	uint16				cur_index = 0;
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

	item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;

	if (PNULL != data_ptr)
	{
		cur_index = data_ptr->item_index;	
		switch(cur_index)
		{
		case 0:
			GetListSplitItemDate(TXT_QB_SYSTEM_DEFAULT, &item_data);
			break;
		case 1:
			GetListDefaulItemtData(IMAGE_THEME_ICON_BG, TXT_COMMON_CLASSIC, &item_data);
			if (SYSTEM_DEFAULT == MMIQBTHEME_GetCurIconBgType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		case 2:
			GetListDefaulItemtData(IMAGE_THEME_ICON_BG_NONE, TXT_QB_TRANSPARENT, &item_data);
			if (DEFAULT_NONE == MMIQBTHEME_GetCurIconBgType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		case 3:
			GetListSplitItemDate(TXT_QB_THEME_DEFAULT, &item_data);
			break;
		case 4:
			GetListDynamicItemData(TXT_QB_THEME_DEFAULT, PNULL, &item_data);
			if (THEME_DEFAULT == MMIQBTHEME_GetCurIconBgType())
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		default:
			break;
		}
		
		GUILIST_SetItemData(ctrl_id, &item_data, cur_index);
	}	
}

/*******************************************************************/
//  Interface:		QBThemeIconBgListboxInit
//  Description : 	QBThemeIconBgListboxInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeIconBgListboxInit(
										  MMI_CTRL_ID_T ctrl_id
										  )
{
	uint16		item_count = QB_INDEX_FIX_ITEM_MAX;
	
	if (PNULL != MMIQBTHEME_GetCurThemeInfo())
	{
		item_count += 2;
	}
	
	GUILIST_SetMaxItem(ctrl_id, item_count, TRUE);
	
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	if (item_count > QB_INDEX_FIX_ITEM_MAX)
	{
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleQbThemeIconBgListWinMsg
//  Description : 	HandleQbThemeIconBgListWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeIconBgListWinMsg(
												 MMI_WIN_ID_T      win_id, 	
												 MMI_MESSAGE_ID_E  msg_id,
												 DPARAM            param
												 )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_ICON_BG_LISTWIN_LISTBOX_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		QBThemeIconBgListboxInit(ctrl_id);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		HandleIconBgListNeedData(ctrl_id, (GUILIST_NEED_ITEM_DATA_T *)param);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		GetListDynamicItemContent(ctrl_id, QB_IMG_ICON_BG, MMIQBTHEME_GetCurThemeInfo(), (GUILIST_NEED_ITEM_CONTENT_T *)param);
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
		{
			uint16 list_index = GUILIST_GetCurItemIndex(ctrl_id);		
			if (QB_INDEX_FIX_ITEM_MAX > list_index)
			{
				list_index -= QB_INDEX_SYSTEM_ITEM;
			}
			else
			{
				list_index -= QB_INDEX_DYNAMIC_LABEL;
			}
			
			MMK_CreateWin((uint32 *)QBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)list_index);
		}	
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// wallpaper list win
/*******************************************************************/
//  Interface:		HandleWallpaperListNeedData
//  Description : 	HandleWallpaperListNeedData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleWallpaperListNeedData(
									   MMI_CTRL_ID_T		ctrl_id, 
									   GUILIST_NEED_ITEM_DATA_T *data_ptr
									   )
{
	uint16			cur_index = 0;
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	wchar* wallpaper_ptr = MMIQBTHEME_GetCurWallpaper();
	wchar* cur_name_ptr = PNULL;
	MMISET_WALLPAPER_SET_T wallpaper_info = {0};
	
	MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[3].item_data.image_id = QBTHEME_IMAGE_NULL;
	
	if (PNULL != data_ptr)
	{
		cur_index = data_ptr->item_index;	
		switch(cur_index)
		{
		case 0:
			GetListSplitItemDate(TXT_QB_SYSTEM_DEFAULT ,&item_data);
			break;
		case 1:
			GetListDefaulItemtData(IMAGE_THEME_BG_PREVIEW, TXT_COMMON_CLASSIC, &item_data);
			if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type
				&& (PNULL == wallpaper_ptr || 0 == MMIAPICOM_Wstrlen(wallpaper_ptr)))
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		case 2:
			GetListSplitItemDate(TXT_QB_THEME_DEFAULT ,&item_data);
			break;
		default:
			cur_name_ptr = MMIQBTHEME_GetWallpaperName(cur_index - QB_INDEX_FIX_ITEM_MAX);
			GetListDynamicItemData(PNULL, cur_name_ptr, &item_data);
			if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type
				&& 0 == MMIAPICOM_Wstrcmp(wallpaper_ptr, cur_name_ptr))
			{
				item_data.item_content[3].item_data.image_id = IMAGE_QBTHEME_MARK;
			}
			break;
		}
		
		GUILIST_SetItemData(ctrl_id, &item_data, cur_index);
	}	
}

/*******************************************************************/
//  Interface:		HandleWallpaperListNeedContent
//  Description : 	HandleWallpaperListNeedContent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleWallpaperListNeedContent(
									  MMI_CTRL_ID_T ctrl_id, 
									  GUILIST_NEED_ITEM_CONTENT_T *content_ptr
									  )
{
	GUIITEM_CONTENT_T   item_content = {0};
	GUIANIM_FILE_INFO_T file_info = {0};
	wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	QBTHEME_PACKAGE_INFO_T *pkg_info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	if (PNULL == content_ptr || PNULL == pkg_info_ptr)
	{
		return;
	}
	
	MMIQBTHEME_GetWallpaerPreviewPath(pkg_info_ptr, content_ptr->item_index - QB_INDEX_DYNAMIC_ITEM_START, anim_file);
	
	file_info.full_path_wstr_ptr = anim_file;
	file_info.full_path_wstr_len = (uint16)MMIAPICOM_Wstrlen(file_info.full_path_wstr_ptr);
	
	item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
	item_content.item_data.anim_path_ptr = &file_info;
	
	GUILIST_SetItemContent(ctrl_id, &item_content, content_ptr->item_index, content_ptr->item_content_index);
}

/*******************************************************************/
//  Interface:		QBThemeWallpaperListboxInit
//  Description : 	QBThemeWallpaperListboxInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E QBThemeWallpaperListboxInit(
											   MMI_CTRL_ID_T ctrl_id
											   )
{
	uint32      i = 0;	
	uint16 item_count	= QB_INDEX_FIX_ITEM_MAX - 1;
	uint32 dynamic_item = MMIQBTHEME_GetThemeWallpaperCount();
	
	if (dynamic_item > QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE - 1)
	{
		item_count += QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE;
	}
	else if (dynamic_item > 0)
	{
		item_count += (uint16)dynamic_item + 1;
	}	 
	
	GUILIST_SetMaxItem(ctrl_id, item_count, TRUE);
	
	// 0, 1
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
	AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);
	
	if (item_count > QB_INDEX_FIX_ITEM_MAX)
	{
		// 2
		AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT);
		
		// 3...
		for (i = QB_INDEX_FIX_ITEM_MAX; i < item_count; i++)
		{
			AppendListThemeItem(ctrl_id, GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON);	
		}
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleQbThemeWallpaperListWinMsg
//  Description : 	HandleQbThemeWallpaperListWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeWallpaperListWinMsg(
													MMI_WIN_ID_T      win_id, 	
													MMI_MESSAGE_ID_E  msg_id,
													DPARAM            param
													)
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_WALLPAPER_LISTWIN_LISTBOX_CTRL_ID;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		MMIQBTHEME_WallpaperListInit(MMIQBTHEME_GetCurThemeInfo());
		QBThemeWallpaperListboxInit(ctrl_id);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		HandleWallpaperListNeedData(ctrl_id, (GUILIST_NEED_ITEM_DATA_T *)param);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		HandleWallpaperListNeedContent(ctrl_id, (GUILIST_NEED_ITEM_CONTENT_T *)param);
		break;
	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
		{
			uint16 list_index = GUILIST_GetCurItemIndex(ctrl_id );		
			
			if (QB_INDEX_SYSTEM_ITEM == list_index)
			{
				list_index = 0;
			}
			else
			{
				list_index -= QB_INDEX_DYNAMIC_LABEL;
			}

			MMK_CreateWin((uint32 *)QBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)list_index);
		}	
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		MMIQBTHEME_DestroyWallpaperList();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// theme preview win
/*******************************************************************/
//  Interface:		SetOsdLayerDisplayStatus
//  Description : 	SetOsdLayerDisplayStatus
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetOsdLayerDisplayStatus(
									BOOLEAN is_display
									)
{
	UILAYER_APPEND_BLT_T append_info = {0};
	
	if ((0 != s_qbtheme_up_panel_layer.block_id) 
        && UILAYER_IsLayerActive(&s_qbtheme_up_panel_layer))
    {
		append_info.layer_level = UILAYER_LEVEL_NORMAL;
		append_info.lcd_dev_info = s_qbtheme_up_panel_layer;
		
		if (is_display)
		{
			UILAYER_AppendBltLayer(&append_info);		
		}
		else
		{
			UILAYER_RemoveBltLayer(&append_info);
		}
	}
	
	if ((0 != s_qbtheme_down_panel_layer.block_id) 
        && UILAYER_IsLayerActive(&s_qbtheme_down_panel_layer))
    {
		append_info.layer_level = UILAYER_LEVEL_NORMAL;
		append_info.lcd_dev_info = s_qbtheme_down_panel_layer;
		
		if (is_display)
		{
			UILAYER_AppendBltLayer(&append_info);		
		}
		else
		{
			UILAYER_RemoveBltLayer(&append_info);
		}
	}
}

/*******************************************************************/
//  Interface:		ClearOsdPanelLayer
//  Description : 	ClearOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ClearOsdPanelLayer(
							  GUI_LCD_DEV_INFO *layer_ptr
							  )
{
	GUI_RECT_T			 rc_layer = {0};
	
	rc_layer = UILAYER_GetLayerRect(&s_qbtheme_up_panel_layer);
	UILAYER_ClearRect(layer_ptr, rc_layer);
	
	rc_layer = UILAYER_GetLayerRect(&s_qbtheme_down_panel_layer);
	UILAYER_ClearRect(layer_ptr, rc_layer);
}

/*******************************************************************/
//  Interface:		CopyOsdPanelLayer
//  Description : 	CopyOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CopyOsdPanelLayer(
							 GUI_LCD_DEV_INFO *layer_ptr, 
							 BOOLEAN is_compound
							 )
{
	GUI_RECT_T			 rc_layer = {0};
	
	rc_layer = UILAYER_GetLayerRect(&s_qbtheme_up_panel_layer);
	UILAYER_ClearRect(layer_ptr, rc_layer);
	UILAYER_BltLayerToLayer(layer_ptr, &s_qbtheme_up_panel_layer, &rc_layer, is_compound);
	
	rc_layer = UILAYER_GetLayerRect(&s_qbtheme_down_panel_layer);
	UILAYER_ClearRect(layer_ptr, rc_layer);
	UILAYER_BltLayerToLayer(layer_ptr, &s_qbtheme_down_panel_layer, &rc_layer, is_compound);
}

/*******************************************************************/
//  Interface:		DestroyOsdPanelLayer
//  Description : 	DestroyOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyOsdPanelLayer(void)
{
	UILAYER_ReleaseLayer(&s_qbtheme_up_panel_layer);
	s_qbtheme_up_panel_layer.block_id = UILAYER_NULL_HANDLE;
	
	UILAYER_ReleaseLayer(&s_qbtheme_down_panel_layer);
	s_qbtheme_down_panel_layer.block_id = UILAYER_NULL_HANDLE;
}

/*******************************************************************/
//  Interface:		CreateOsdPanelLayer
//  Description : 	CreateOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CreateOsdPanelLayer(
							   MMI_WIN_ID_T      win_id,
							   BOOLEAN			is_append
							   )
{
	UILAYER_CREATE_T    create_info = {0};
	GUI_RECT_T			rc_layer = {0};
	
    int16				layer_height = QB_PANEL_LAYER_HEIGHT;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
	
	if (UILAYER_IsMultiLayerEnable())
    {
		DestroyOsdPanelLayer();
		
        //create up layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        UILAYER_CreateLayer(&create_info, &s_qbtheme_up_panel_layer);
		
		//create down layer
        create_info.offset_y = (int16)(lcd_height - layer_height);
		UILAYER_CreateLayer(&create_info, &s_qbtheme_down_panel_layer);
		
        //set color key
        UILAYER_SetLayerColorKey(&s_qbtheme_up_panel_layer, TRUE,UILAYER_TRANSPARENT_COLOR);
		UILAYER_Clear(&s_qbtheme_up_panel_layer);
		
		UILAYER_SetLayerColorKey(&s_qbtheme_down_panel_layer, TRUE,UILAYER_TRANSPARENT_COLOR);
		UILAYER_Clear(&s_qbtheme_down_panel_layer);
		
		//append layer
		if (is_append)
		{
			SetOsdLayerDisplayStatus(TRUE);
		}
		
		//set bg
#ifdef GUIF_SOFTKEY_PDASTYLE		
		rc_layer = UILAYER_GetLayerRect(&s_qbtheme_up_panel_layer);
		GUIRES_DisplayImg(PNULL, &rc_layer, PNULL, win_id, IMAGE_PDASOFTKEY_BG, &s_qbtheme_up_panel_layer);
		
		rc_layer = UILAYER_GetLayerRect(&s_qbtheme_down_panel_layer);
		GUIRES_DisplayImg(PNULL, &rc_layer, PNULL, win_id, IMAGE_PDASOFTKEY_BG, &s_qbtheme_down_panel_layer);		
#endif
    }	
}

/*******************************************************************/
//  Interface:		DisplayOsdPanelLayer
//  Description : 	DisplayOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DisplayOsdPanelLayer(void)
{
	if (!s_qbtheme_is_display_osd)
	{
		uint32 i = 0;
		int16 offset_x = 0;
		int16 up_offset_y = 0;
		int16 down_offset_y = 0;
		
		UILAYER_GetLayerPosition(&s_qbtheme_up_panel_layer, &offset_x, &up_offset_y);
		UILAYER_GetLayerPosition(&s_qbtheme_down_panel_layer, &offset_x, &down_offset_y);

		for (; i < (QB_PANEL_LAYER_HEIGHT + QB_PANEL_ANIM_FRAME_STEP - 1)/QB_PANEL_ANIM_FRAME_STEP; i++)
		{
			up_offset_y += QB_PANEL_ANIM_FRAME_STEP;
			UILAYER_SetLayerPosition(&s_qbtheme_up_panel_layer, 0, up_offset_y);
			
			down_offset_y -= QB_PANEL_ANIM_FRAME_STEP;
			UILAYER_SetLayerPosition(&s_qbtheme_down_panel_layer, 0, down_offset_y);
			
			MMITHEME_UpdateRect();
			SCI_Sleep(QB_PANEL_DELAY_TICK);
		}
		
		s_qbtheme_is_display_osd = TRUE;
	}
}

/*******************************************************************/
//  Interface:		HideOsdPanelLayer
//  Description : 	HideOsdPanelLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HideOsdPanelLayer(void)
{
	if (s_qbtheme_is_display_osd)
	{
		uint32 i = 0;
		int16 offset_x = 0;
		int16 up_offset_y = 0;
		int16 down_offset_y = 0;
		
		UILAYER_GetLayerPosition(&s_qbtheme_up_panel_layer, &offset_x, &up_offset_y);
		UILAYER_GetLayerPosition(&s_qbtheme_down_panel_layer, &offset_x, &down_offset_y);
		
		for (; i < (QB_PANEL_LAYER_HEIGHT + QB_PANEL_ANIM_FRAME_STEP - 1)/QB_PANEL_ANIM_FRAME_STEP; i++)
		{
			up_offset_y -= QB_PANEL_ANIM_FRAME_STEP;
			UILAYER_SetLayerPosition(&s_qbtheme_up_panel_layer, 0, up_offset_y);
			
			down_offset_y += QB_PANEL_ANIM_FRAME_STEP;
			UILAYER_SetLayerPosition(&s_qbtheme_down_panel_layer, 0, down_offset_y);
			
			MMITHEME_UpdateRect();
			SCI_Sleep(QB_PANEL_DELAY_TICK);
		}
		
		s_qbtheme_is_display_osd = FALSE;
	}
}

/*******************************************************************/
//  Interface:		MoveLabelPos
//  Description : 	MoveLabelPos
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void MoveLabelPos(
						MMI_CTRL_ID_T label_id, 
						int16 offset_x, 
						int16 offset_y
						)
{
	GUI_RECT_T rc_label = {0};
	
	GUIAPICTRL_GetRect(label_id, &rc_label);
	rc_label.left += offset_x;
	rc_label.right += offset_x;
	rc_label.top += offset_y;
	rc_label.bottom += offset_y;
	GUIAPICTRL_SetRect(label_id, &rc_label);
}

/*******************************************************************/
//  Interface:		HandlePanelLayerTpUp
//  Description : 	HandlePanelLayerTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandlePanelLayerTpUp(
								MMI_CTRL_ID_T label1,
								MMI_CTRL_ID_T label2,
								MMI_CTRL_ID_T button1,
								MMI_CTRL_ID_T button2,
								MMI_CTRL_ID_T button3,
								MMI_CTRL_ID_T button4
								)
{
	if(s_qbtheme_is_display_osd)
	{
		HideOsdPanelLayer();
		MoveLabelPos(label1, 0, -QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(label2, 0, -QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button1, 0, -QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button2, 0, -QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button3, 0, QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button4, 0, QB_PANEL_LAYER_HEIGHT);
	}
	else
	{
		DisplayOsdPanelLayer();
		MoveLabelPos(label1, 0, QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(label2, 0, QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button1, 0, QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button2, 0, QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button3, 0, -QB_PANEL_LAYER_HEIGHT);
		MoveLabelPos(button4, 0, -QB_PANEL_LAYER_HEIGHT);
	}
}

/*******************************************************************/
//  Interface:		InitAnimCtrlRect
//  Description : 	InitAnimCtrlRect
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void InitAnimCtrlRect(
							MMI_CTRL_ID_T ctrl_id,
							int16         index
							)
{
	uint16          lcd_width = 0;
    uint16          lcd_height = 0;
	GUI_RECT_T		ctrl_rect = {0};
    GUI_RECT_T      display_rect = {0};
	
	//get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
	
	// anim rect
	ctrl_rect.top = 0;
	ctrl_rect.bottom = (int16)(lcd_height - 1);
	ctrl_rect.left = index * (int16)(lcd_width + QB_THEME_GLIDE_FRAME_GAP);
	ctrl_rect.right = (index + 1) * (int16)lcd_width + index * QB_THEME_GLIDE_FRAME_GAP - 1;
	
	GUIANIM_SetCtrlRect(ctrl_id, &ctrl_rect);
	if (0 == index)
	{
		GUIANIM_SetDisplayRect(ctrl_id, &ctrl_rect, FALSE);
	}
	else
	{
		GUIANIM_SetDisplayRect(ctrl_id, &display_rect, FALSE);	
	}
}

/*******************************************************************/
//  Interface:		SetAnimCtrlDisplayRect
//  Description : 	SetAnimCtrlDisplayRect
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetAnimCtrlDisplayRect(
								    MMI_CTRL_ID_T     ctrl_id,
									int16             move_x,
									BOOLEAN			  is_update
								)
{
    GUI_RECT_T      ctrl_rect = {0};
    GUI_RECT_T      display_rect = {0};
	
    //get control rect
    GUIAPICTRL_GetRect(ctrl_id, &ctrl_rect);
	
    ctrl_rect.left  = (int16)(ctrl_rect.left + move_x);
    ctrl_rect.right = (int16)(ctrl_rect.right + move_x);
	
    //set control rect
    GUIANIM_SetCtrlRect(ctrl_id, &ctrl_rect);
	
    //get display rect
    display_rect = MMITHEME_GetFullScreenRect();
    if (!GUI_IntersectRect(&display_rect, ctrl_rect, display_rect))
    {
        display_rect.left   = 0;
        display_rect.top    = 0;
        display_rect.right  = 0;
        display_rect.bottom = 0;
    }
	
    //set display rect
    GUIANIM_SetDisplayRect(ctrl_id, &display_rect, is_update);
}

LOCAL void SetThemePreviewNameLabel(
									 MMI_WIN_ID_T       win_id,
									 MMI_CTRL_ID_T		label_name,
									 MMI_TEXT_ID_T      text_id,
									 wchar*				ws_text,
									 QBTHEME_PACKAGE_INFO_T *info_ptr,
									 uint16  max_page,
									 BOOLEAN is_update
									 )
{
	MMI_STRING_T str_name = {0};
	GUI_RECT_T rc_name = {0};
	uint8 c_page[QB_THEME_LABEL_MAX_LEN + 1] = {0};
	wchar ws_page[QB_THEME_LABEL_MAX_LEN + 1] = {0};
	wchar ws_name[QBTHEME_NAME_MAX_LEN + 1 + QB_THEME_LABEL_MAX_LEN] = {0};
	uint16 str_len = 0;
	
	GUILABEL_GetRect(label_name, &rc_name);
	
	if (0 != text_id)
	{
		MMI_GetLabelTextByLang(text_id, &str_name);
	}
	else if (PNULL != ws_text)
	{
		str_name.wstr_ptr = ws_text;
		str_name.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_name.wstr_ptr);
	}
	else if (PNULL != info_ptr)
	{
		GetQbThemeDispName(info_ptr, &str_name);
	}
	else
	{
		MMI_GetLabelTextByLang(TXT_COMMON_CLASSIC, &str_name);
	}
	
	ClearOsdRectBg(win_id, rc_name);
	
	_snprintf((char*)c_page, QB_THEME_LABEL_MAX_LEN, " %u/%u", (uint32)s_qbtheme_anim_cur_index + 1, (uint32)max_page);
	MMIAPICOM_StrToWstr(c_page, ws_page);
	str_len = (str_name.wstr_len < QBTHEME_NAME_MAX_LEN ? str_name.wstr_len : QBTHEME_NAME_MAX_LEN);
	MMIAPICOM_Wstrncpy(ws_name, str_name.wstr_ptr, str_len);
	MMIAPICOM_Wstrncat(ws_name, ws_page, QB_THEME_LABEL_MAX_LEN);
	str_name.wstr_ptr = ws_name;
	str_name.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_name.wstr_ptr);
	
	GUILABEL_SetText(label_name, &str_name, is_update);	
}

/*******************************************************************/
//  Interface:		SetThemePreviewSizeLabel
//  Description : 	SetThemePreviewSizeLabel
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetThemePreviewSizeLabel(
									 MMI_WIN_ID_T      win_id,
									 MMI_CTRL_ID_T  label_size,
									 QBTHEME_PACKAGE_INFO_T *info_ptr,
									 uint32  size,
									 BOOLEAN is_update
									 )
{
	MMI_STRING_T str_size = {0};
	GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
	uint8 c_size[QB_THEME_LABEL_MAX_LEN + 1] = {0};
	wchar ws_size[QB_THEME_LABEL_MAX_LEN + 1] = {0};
	
	GUILABEL_GetRect(label_size, &rc_size);	
	ClearOsdRectBg(win_id, rc_size);
	
	if (PNULL != info_ptr)
	{
		if (0 != size)
		{
			_snprintf((char*)c_size, QB_THEME_LABEL_MAX_LEN, "%1.3fM", (double)size/1024/1024);
		}
		else
		{
			_snprintf((char*)c_size, QB_THEME_LABEL_MAX_LEN, "%1.3fM", (double)info_ptr->pkg_size/1024/1024);
		}
		
		MMIAPICOM_StrToWstr(c_size, ws_size);
		str_size.wstr_ptr = ws_size;
		str_size.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_size.wstr_ptr);
		GUILABEL_SetText(label_size, &str_size, is_update);		
	}
	else
	{
		GUILABEL_SetTextById(label_size, TXT_QB_SYSTEM_DEFAULT, is_update);
	}
}

/*******************************************************************/
//  Interface:		SetThemePreviewAnims
//  Description : 	SetThemePreviewAnims
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetThemePreviewAnims(QBTHEME_PACKAGE_INFO_T *info_ptr, BOOLEAN is_update)
{
	InitAnimCtrlRect(QBTHEME_PREVIEW1_ANIM_CTRL_ID, 0);
	InitAnimCtrlRect(QBTHEME_PREVIEW2_ANIM_CTRL_ID, 1);
	InitAnimCtrlRect(QBTHEME_PREVIEW3_ANIM_CTRL_ID, 2);
	
	if (PNULL == info_ptr)
	{
		SetAnimCtrlParam(QBTHEME_PREVIEW1_ANIM_CTRL_ID, IMAGE_THEME_PREVIEW1, PNULL, TRUE, is_update);
		SetAnimCtrlParam(QBTHEME_PREVIEW2_ANIM_CTRL_ID, IMAGE_THEME_PREVIEW2, PNULL, TRUE, is_update);
		SetAnimCtrlParam(QBTHEME_PREVIEW3_ANIM_CTRL_ID, IMAGE_THEME_PREVIEW3, PNULL, TRUE, is_update);
	}
	else
	{
		wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		
		MMIQBTHEME_GetImagePath(info_ptr, QB_IMG_PREVIEW1, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		SetAnimCtrlParam(QBTHEME_PREVIEW1_ANIM_CTRL_ID, 0, anim_file, FALSE, is_update);
		
		SCI_MEMSET(anim_file, 0, sizeof(anim_file));
		MMIQBTHEME_GetImagePath(info_ptr, QB_IMG_PREVIEW2, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		SetAnimCtrlParam(QBTHEME_PREVIEW2_ANIM_CTRL_ID, 0, anim_file, FALSE, is_update);
		
		SCI_MEMSET(anim_file, 0, sizeof(anim_file));
		MMIQBTHEME_GetImagePath(info_ptr, QB_IMG_PREVIEW3, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		SetAnimCtrlParam(QBTHEME_PREVIEW3_ANIM_CTRL_ID, 0, anim_file, FALSE, is_update);
	}
}

/*******************************************************************/
//  Interface:		ResetQBThemePreview
//  Description : 	ResetQBThemePreview
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ResetQBThemePreview()
{
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
	
	if (s_qbtheme_theme_cur_index > 0)
	{
		info_ptr = MMIQBTHEME_GetInfoByIndex(s_qbtheme_theme_cur_index - 1);
	}
	
	SetThemePreviewAnims(info_ptr, TRUE);
	s_qbtheme_anim_cur_index = 0;
	
	SetThemePreviewNameLabel(MMIQBTHEME_PREVIEW_GLIDE_WIN_ID, QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
		info_ptr, QB_THEME_PREVIEW_MAX_PAGE, TRUE);
	SetThemePreviewSizeLabel(MMIQBTHEME_PREVIEW_GLIDE_WIN_ID, QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr,0, TRUE);
	
}

/*******************************************************************/
//  Interface:		NextQBTheme
//  Description : 	NextQBTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E NextQBTheme(void)
{
	uint32 theme_count = MMIQBTHEME_GetDownloadCount() + 1;
	
	if (s_qbtheme_theme_cur_index < theme_count - 1)
	{
		BOOLEAN is_end = FALSE;
		
		is_end = (BOOLEAN)(++s_qbtheme_theme_cur_index == theme_count -1);
		
		SetButtonGray(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
		SetButtonGray(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, FALSE, TRUE);
		GUIBUTTON_SetGrayed(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, FALSE, PNULL, TRUE);
		
		ResetQBThemePreview();		
		//update
		MMITHEME_UpdateRect();
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		PreQBTheme
//  Description : 	PreQBTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E PreQBTheme(void)
{	
	if (s_qbtheme_theme_cur_index > 0)
	{
		BOOLEAN is_start = FALSE;
		
		is_start = (--s_qbtheme_theme_cur_index == 0);
		SetButtonGray(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, FALSE, TRUE);
		SetButtonGray(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		GUIBUTTON_SetGrayed(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, is_start, PNULL, TRUE);
		
		ResetQBThemePreview();		
		//update
		MMITHEME_UpdateRect();
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyTheme
//  Description : 	ApplyTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyTheme(
                              QBTHEME_PACKAGE_INFO_T* info_ptr
                              )
{
    uint8 icon_type = THEME_DEFAULT;
    uint8 icon_bg_type = THEME_DEFAULT;
    uint8 font_type = THEME_DEFAULT;
    uint8 old_font_type = THEME_DEFAULT;
    wchar new_theme[QBTHEME_NAME_EN_MAX_LEN + 1] = {0};
    wchar* wallpaper_ptr = MMIQBTHEME_GetCurWallpaper();
    wchar* theme_ptr = MMIQBTHEME_GetCurrentTheme();
    
    if (PNULL == theme_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    if (PNULL != info_ptr)
    {
        MMIAPICOM_Wstrncpy(new_theme, info_ptr->name_en, QBTHEME_NAME_EN_MAX_LEN);
    }
    ResetWallpaperSet();
    if (0 == MMIAPICOM_Wstrcmp(theme_ptr, new_theme))
    {
        MMK_ReturnIdleWin();
        return MMI_RESULT_TRUE;
    }
    
    if (PNULL == info_ptr)
    {
        icon_type = SYSTEM_DEFAULT;
        icon_bg_type = SYSTEM_DEFAULT;
        font_type = SYSTEM_DEFAULT;
    }
    MMIQBTHEME_SetCurIconType(icon_type);
    MMINV_WRITE(QBTHEME_NV_MAINMENU_ICON, &icon_type);
    MMIQBTHEME_SetCurIconBgType(icon_bg_type);
    MMINV_WRITE(QBTHEME_NV_ICON_BG, &icon_type);
    
    if (0 == MMIQBTHEME_GetPkgFontSize(info_ptr))
    {
        font_type = SYSTEM_DEFAULT;
    }
    old_font_type = MMIQBTHEME_GetCurFontType();
    MMIQBTHEME_SetCurFontType(font_type);
    MMINV_WRITE(QBTHEME_NV_FONT, &font_type);
    if (SYSTEM_DEFAULT != font_type || SYSTEM_DEFAULT != old_font_type)
    {
        if (PNULL == info_ptr)
        {
            MMIRES_UnloadFileFont();
        }
        else
        {
            MMIQBTHEME_LoadThemeFont(info_ptr);
        }
    }
    
    if (PNULL != wallpaper_ptr)
    {
        SCI_MEMSET(wallpaper_ptr, 0, sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
        if (PNULL != info_ptr)
        {
            MMIAPICOM_Wstrcpy(wallpaper_ptr, L"default");
        }
        else
        {
            MMIAPICOM_Wstrcpy(wallpaper_ptr, L"");
        }
        
        MMINV_WRITE(QBTHEME_NV_WALLPAPER, wallpaper_ptr);
    }
    
    {
        SCI_MEMSET(theme_ptr, 0, sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
        MMIAPICOM_Wstrncpy(theme_ptr, new_theme, QBTHEME_NAME_EN_MAX_LEN);
        MMINV_WRITE(QBTHEME_NV_CUR_THEME, theme_ptr);
    }
    
    // clear res cache
    MMI_FreeAllRes();
    MMIQBTHEME_InitResHandle(info_ptr);	
    
    MMK_ReturnIdleWin();
    MMIPUB_OpenAlertWaitingWin(TXT_QB_SWITCHING_THEME);
    
    return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyQBTheme
//  Description : 	ApplyQBTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyQBTheme(void)
{
    QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
    
    if (s_qbtheme_theme_cur_index > 0)
    {
        info_ptr = MMIQBTHEME_GetInfoByIndex(s_qbtheme_theme_cur_index - 1);
    }
    
    return ApplyTheme(info_ptr);
}

/*******************************************************************/
//  Interface:		HandleQbQueryWinMsg
//  Description : 	HandleQbQueryWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbQueryWinMsg(
									   MMI_WIN_ID_T        win_id,    //IN:
									   MMI_MESSAGE_ID_E    msg_id,    //IN:
									   DPARAM              param    //IN:
									   )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
	MMIPUB_INFO_T       *win_info_ptr   = PNULL;
	QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
	
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
		{
			win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
			if (PNULL != win_info_ptr)
			{
				info_ptr = (QBTHEME_PACKAGE_INFO_T*)(win_info_ptr->user_data);
			}
			
			MMIPUB_CloseQuerytWin(&win_id);
			if(PNULL != info_ptr && MMIQBTHEME_Uninstall(info_ptr->name_en))
			{
				MMK_PostMsg(MMIQBTHEME_LIST_WIN_ID, MSG_UPDATE_QBTHEME_LIST, 0, 0);
				MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
				PreQBTheme();
				if (0 == MMIQBTHEME_GetDownloadCount())
				{
					SetButtonGray(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
						IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, TRUE);
				}
			}
			else
			{
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
		}
        break;
		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&win_id);
        break;
		
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*******************************************************************/
//  Interface:		RemoveQBTheme
//  Description : 	RemoveQBTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E RemoveQBTheme(void)
{
	QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
	wchar* theme_ptr = MMIQBTHEME_GetCurrentTheme();
	
	if (PNULL == theme_ptr)
	{
		return MMI_RESULT_FALSE;
	}
	
	if (s_qbtheme_theme_cur_index > 0)
	{
		info_ptr = MMIQBTHEME_GetInfoByIndex(s_qbtheme_theme_cur_index - 1);
		if (PNULL != info_ptr)
		{
			if (0 == MMIAPICOM_Wstrcmp(theme_ptr, info_ptr->name_en))
			{
				MMIPUB_OpenAlertFailWin( TXT_QB_USING_NO_REMOVE);
			}
			else
			{
				MMI_WIN_ID_T query_win_id = MMIQBTHEME_QUERY_WIN_ID;
				MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY,\
					&query_win_id, HandleQbQueryWinMsg, (uint32)info_ptr);
			}
		}
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		QBThemePreviewInit
//  Description : 	QBThemePreviewInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemePreviewInit(
							  MMI_WIN_ID_T  win_id, 
							  QBTHEME_PACKAGE_INFO_T *info_ptr
							  )
{
    GUI_BG_T   bg_btn	= {0};
    GUI_RECT_T rc_btn_l = QB_PANEL_LEFT_BTN_RECT;
    GUI_RECT_T rc_btn_r = QB_PANEL_RIGHT_BTN_RECT;
    GUI_RECT_T rc_name = QB_PANEL_NAME_LABEL_RECT;
    GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
    GUI_RECT_T rc_key_l = QB_PANEL_LEFT_KEY_RECT;
    GUI_RECT_T rc_key_r = QB_PANEL_RIGHT_KEY_RECT;
    
    if (!s_qbtheme_preview_is_readonly)
    {
        s_qbtheme_is_display_osd = TRUE;
        
        CreateOsdPanelLayer(win_id, TRUE);
        
        // left btn
        GUIBUTTON_SetRect(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, &rc_btn_l);
        bg_btn.bg_type = GUI_BG_IMG;
        bg_btn.img_id = IMAGE_QBTHEME_BTN_LEFT_ON;	
        GUIBUTTON_SetPressedBg(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, &bg_btn);
        GUIBUTTON_SetCallBackFunc(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, PreQBTheme);
        GUIBUTTON_SetRunSheen(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, FALSE);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
        
        // right btn
        GUIBUTTON_SetRect(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, &rc_btn_r);
        bg_btn.bg_type = GUI_BG_IMG;
        bg_btn.img_id = IMAGE_QBTHEME_BTN_RIGHT_ON;	
        GUIBUTTON_SetPressedBg(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, &bg_btn);
        GUIBUTTON_SetCallBackFunc(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, NextQBTheme);
        GUIBUTTON_SetRunSheen(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, FALSE);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
        
        // label
        bg_btn.bg_type = GUI_BG_COLOR;
        bg_btn.color = MMI_TRANSPARENCE_COLOR;
        GUILABEL_SetBg(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, &bg_btn);
        GUILABEL_SetRect(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, &rc_name, FALSE);
        GUILABEL_SetFont(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
        GUILABEL_SetBg(QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, &bg_btn);
        GUILABEL_SetRect(QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, &rc_size, FALSE);
        GUILABEL_SetFont(QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
        
        SetThemePreviewNameLabel(win_id, QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
            info_ptr, QB_THEME_PREVIEW_MAX_PAGE, FALSE);
        SetThemePreviewSizeLabel(win_id, QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0, FALSE);
        
        // key left
        GUIBUTTON_SetRect(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, &rc_key_l);
#ifdef GUIF_SOFTKEY_PDASTYLE    
        bg_btn.bg_type = GUI_BG_IMG;
        bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
        GUIBUTTON_SetPressedBg(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, &bg_btn);
#endif    
        SetButtonFont(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
        GUIBUTTON_SetTextId(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, TXT_QB_THEME_APPLY);
        GUIBUTTON_SetCallBackFunc(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, ApplyQBTheme);
        GUIBUTTON_SetRunSheen(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, FALSE);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
        
        // key right
        GUIBUTTON_SetRect(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, &rc_key_r);
#ifdef GUIF_SOFTKEY_PDASTYLE    
        bg_btn.bg_type = GUI_BG_IMG;
        bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
        GUIBUTTON_SetPressedBg(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, &bg_btn);
#endif    
        SetButtonFont(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
        GUIBUTTON_SetTextId(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, TXT_DELETE);
        GUIBUTTON_SetCallBackFunc(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, RemoveQBTheme);
        GUIBUTTON_SetRunSheen(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, FALSE);
        MMK_SetCtrlLcdDevInfo(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
        
        // is_start gray pre/remvoe,is_end gray next
        if (0 == s_qbtheme_theme_cur_index)
        {
            SetButtonGray(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
                IMAGE_QBTHEME_BTN_LEFT_GRAY, TRUE, FALSE);
#ifdef GUIF_SOFTKEY_PDASTYLE        
            SetButtonGray(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID, IMAGE_PDASOFTKEY_BUTTON, \
                IMAGE_PDASOFTKEY_GRAYED_BUTTON, TRUE, FALSE);
#endif
        }
        if(MMIQBTHEME_GetDownloadCount() == (uint32)s_qbtheme_theme_cur_index)
        {
            SetButtonGray(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
                IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, FALSE);
        }
    }
    else
    {
        MMK_DestroyControl(QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID);
        MMK_DestroyControl(QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID);
        MMK_DestroyControl(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID);
        MMK_DestroyControl(QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID);
        MMK_DestroyControl(QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID);
        MMK_DestroyControl(QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID);
    }
    
    CreateReturnBtn(win_id, QBTHEME_PREVIEW_RETURN_BTN_CTRL_ID);

    // preview page
    SetThemePreviewAnims(info_ptr, FALSE);
}

/*******************************************************************/
//  Interface:		IsPointInLayer
//  Description : 	IsPointInLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IsPointInLayer(
							 GUI_LCD_DEV_INFO *layer_ptr, 
							 GUI_POINT_T *pt_ptr
							 )
{
	GUI_RECT_T rc = {0};
	BOOLEAN  bret = FALSE;
	
	if (PNULL == pt_ptr
		|| PNULL == layer_ptr
		|| UILAYER_NULL_HANDLE == layer_ptr->block_id)
	{
		return FALSE;
	}
	
	rc = UILAYER_GetLayerRect(layer_ptr);
	
	bret = GUI_PointIsInRect(*pt_ptr, rc);
	
	return bret;
}

/*******************************************************************/
//  Interface:		HandleAnimGlideTpDown
//  Description : 	HandleAnimGlideTpDown
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimGlideTpDown(
								 DPARAM param
								 )
{
	GUI_POINT_T     pt_down = {0};
	
	//get point
	pt_down.x = MMK_GET_TP_X(param);
	pt_down.y = MMK_GET_TP_Y(param);
	
	//is tp osd
	if (IsPointInLayer(&s_qbtheme_up_panel_layer, &pt_down)
		|| IsPointInLayer(&s_qbtheme_down_panel_layer, &pt_down))
	{
		s_qbtheme_is_tp_panel = TRUE;
	}
	else
	{
		s_qbtheme_is_tp_panel = FALSE;
	}
	
	if (!s_qbtheme_is_tp_panel)
	{
		//get point
		s_qbtheme_tp_start_pt = pt_down;
		s_qbtheme_tp_prev_pt  = s_qbtheme_tp_start_pt;
	}
	
	//glide is move
	s_qbtheme_has_moved = FALSE;	
}

/*******************************************************************/
//  Interface:		CalcMoveWidth
//  Description : 	CalcMoveWidth
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL int16 CalcMoveWidth(
						  int16 offset_x, 
						  uint16 max_page,
						  uint16 grap,
						  uint16* cur_index_ptr
						  )
{
    BOOLEAN         is_prev = FALSE;
    BOOLEAN         is_next = FALSE;
    int16           move_width = 0;
	uint16          lcd_width = 0;
	
	//get lcd width
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
	// add grap
	lcd_width += grap; //QB_THEME_GLIDE_FRAME_GAP;
	
	//set start index
	*cur_index_ptr = s_qbtheme_anim_cur_index;
	
	// current
	if (abs(offset_x) < (lcd_width / QB_THEME_GLIDE_RATIO))
	{
		move_width = -offset_x;
	}
	else // previous or next
	{
		if (0 < offset_x)
		{
			//previous
			is_prev = TRUE;
			move_width = (int16)(lcd_width - offset_x);
			
			//set current index
			if (0 < *cur_index_ptr)
			{
				*cur_index_ptr = *cur_index_ptr - 1;
			}
			else
			{
				//reset
				is_prev = FALSE;
				is_next = FALSE;
				move_width = -offset_x;
			}
		}
		else if (0 > offset_x)
		{
			//next
			is_next = TRUE;
			move_width = (int16)(-offset_x - lcd_width);
			
			//set current index
			if (*cur_index_ptr < (max_page - 1))
			{
				*cur_index_ptr = *cur_index_ptr + 1;
			}
			else
			{
				//reset
				is_prev = FALSE;
				is_next = FALSE;
				move_width = -offset_x;
			}
		}
		else
		{
			move_width = 0;
		}	
	}
	
	return move_width;
}

/*******************************************************************/
//  Interface:		HandleAnimGLideTpUp
//  Description : 	HandleAnimGLideTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimGLideTpUp(
							   MMI_WIN_ID_T win_id, 
							   DPARAM param, 
							   QBTHEME_PACKAGE_INFO_T *info_ptr
							   )
{
    int16           offset_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          i = 0;
    uint16          cur_index = 0;
    //GUI_POINT_T     tp_point = {0};
	
	offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
	
	if (!s_qbtheme_is_tp_panel
		&& 0 != offset_x
		&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
	{
        //get point
        //tp_point.x = MMK_GET_TP_X(param);
        //tp_point.y = MMK_GET_TP_Y(param);
		
		move_width = CalcMoveWidth(offset_x, QB_THEME_PREVIEW_MAX_PAGE, QB_THEME_GLIDE_FRAME_GAP, &cur_index);
		
		//auto move
		if(0 != move_width)
		{
			for (i = 1; i <= QB_THEME_GLIDE_AUTO_FRAME; i++)
			{
				//set move space
				if (i <= (QB_THEME_GLIDE_AUTO_FRAME / 2))
				{
					move_offset = (int16)((2 * move_width * i * i) / 100);
				}
				else
				{
					move_offset = (int16)(move_width - (2 * move_width * (QB_THEME_GLIDE_AUTO_FRAME - i) * (QB_THEME_GLIDE_AUTO_FRAME - i)) / 100);
				}
				
				//set move space
				move_space = (int16)(move_offset - move_all);
				move_all   = (int16)(move_all + move_space);
				
				if (0 != move_space)
				{
					//set control rect and display rect
					SetAnimCtrlDisplayRect(QBTHEME_PREVIEW1_ANIM_CTRL_ID, move_space, TRUE);
					SetAnimCtrlDisplayRect(QBTHEME_PREVIEW2_ANIM_CTRL_ID, move_space, TRUE);
					SetAnimCtrlDisplayRect(QBTHEME_PREVIEW3_ANIM_CTRL_ID, move_space, TRUE);
					
					//update
					MMITHEME_UpdateRect();
				}
			}
		}
		//
		if (cur_index != s_qbtheme_anim_cur_index)
		{
			// modify title label
			s_qbtheme_anim_cur_index = cur_index;
			SetThemePreviewNameLabel(win_id, QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
				info_ptr, QB_THEME_PREVIEW_MAX_PAGE, TRUE);
		}
		
		ResetPreviewStatus();
	}
}

/*******************************************************************/
//  Interface:		HandleQbThemePreviewGlideWinMsg
//  Description : 	HandleQbThemePreviewGlideWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemePreviewGlideWinMsg(
							  MMI_WIN_ID_T      win_id, 	
							  MMI_MESSAGE_ID_E  msg_id,
							  DPARAM            param
							  )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_PREVIEW1_ANIM_CTRL_ID;
	uint16              index       = (uint16)((uint32)MMK_GetWinAddDataPtr(win_id));
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		if (0 < index)
		{
			info_ptr = MMIQBTHEME_GetInfoByIndex(index - 1);
		}
		s_qbtheme_theme_cur_index = index;
		QBThemePreviewInit(win_id, info_ptr);
		MMK_SetActiveCtrl(ctrl_id + index, FALSE);
		break;
	case MSG_LOSE_FOCUS:
		SetOsdLayerDisplayStatus(FALSE);
        SetRetBtnLayerDisplay(FALSE);

        if(s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
        {
            int16 offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
            
            SetAnimCtrlDisplayRect(QBTHEME_PREVIEW1_ANIM_CTRL_ID, -offset_x, TRUE);
            SetAnimCtrlDisplayRect(QBTHEME_PREVIEW2_ANIM_CTRL_ID, -offset_x, TRUE);
            SetAnimCtrlDisplayRect(QBTHEME_PREVIEW3_ANIM_CTRL_ID, -offset_x, TRUE);
            
            ResetPreviewStatus();
        }
		break;
	case MSG_GET_FOCUS:
		SetOsdLayerDisplayStatus(TRUE);
        SetRetBtnLayerDisplay(TRUE);
        
		break;
	case MSG_FULL_PAINT:
		break;
	case MSG_NOTIFY_UPDATE:
        if (!s_qbtheme_preview_is_readonly)
        {
            ClearOsdRectBg(win_id, *(GUI_RECT_T *)param);
        }
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		HandleAnimGlideTpDown(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T     pt_move = {0};
			int16			offset_x = 0;
			
			//get point
			pt_move.x = MMK_GET_TP_X(param);
			pt_move.y = MMK_GET_TP_Y(param);
			
			offset_x = (int16)(pt_move.x - s_qbtheme_tp_prev_pt.x);
			if (!s_qbtheme_is_tp_panel 
				&& 0 != offset_x
				&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
			{
				GUI_RECT_T rc = MMITHEME_GetFullScreenRect();
				GUI_FillRect(MMITHEME_GetDefaultLcdDev(), rc, MMI_BACKGROUND_COLOR);
				
				SetAnimCtrlDisplayRect(QBTHEME_PREVIEW1_ANIM_CTRL_ID, offset_x, TRUE);
				SetAnimCtrlDisplayRect(QBTHEME_PREVIEW2_ANIM_CTRL_ID, offset_x, TRUE);
				SetAnimCtrlDisplayRect(QBTHEME_PREVIEW3_ANIM_CTRL_ID, offset_x, TRUE);
				
				s_qbtheme_tp_prev_pt.x = pt_move.x;
				s_qbtheme_tp_prev_pt.y = pt_move.y;
				s_qbtheme_has_moved = TRUE;
				
				//MMITHEME_UpdateRect();				
			}		
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
		{
			if(!s_qbtheme_has_moved && !s_qbtheme_is_tp_panel && !s_qbtheme_preview_is_readonly)
			{
				HandlePanelLayerTpUp(QBTHEME_PREVIEW_NAME_LABEL_CTRL_ID, QBTHEME_PREVIEW_SIZE_LABEL_CTRL_ID, \
					QBTHEME_PREVIEW_LEFT_BTN_CTRL_ID, QBTHEME_PREVIEW_RIGHT_BTN_CTRL_ID, \
					QBTHEME_PREVIEW_APPLY_BTN_CTRL_ID, QBTHEME_PREVIEW_REMOVE_BTN_CTRL_ID);
			}
			else
			{
				if (s_qbtheme_theme_cur_index > 0)
				{
					info_ptr = MMIQBTHEME_GetInfoByIndex(s_qbtheme_theme_cur_index - 1);
				}
				HandleAnimGLideTpUp(win_id, param, info_ptr);
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		s_qbtheme_anim_cur_index = 0;
		s_qbtheme_theme_cur_index = 0;
        s_qbtheme_preview_is_readonly = FALSE;
		DestroyOsdPanelLayer();
        DestroyRetBtnLayer();
		ResetPreviewStatus();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// icon preview win
/*******************************************************************/
//  Interface:		NextQBIcon
//  Description : 	NextQBIcon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E NextQBIcon(void)
{
	uint16	 item_count = QB_THEME_ICON_PREVIEW_MAX_PAGE;
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	if (s_qbtheme_anim_cur_index < item_count - 1)
	{
		BOOLEAN is_end = FALSE;
		uint16 lcd_width = 0;
		QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_end = (BOOLEAN)(++s_qbtheme_anim_cur_index == item_count -1);
		SetButtonGray(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
		SetButtonGray(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, FALSE, TRUE);
		
		SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, -(lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		if (1 < item_count)
		{
			SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, -(lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		if (is_end)
		{
			info_ptr = MMIQBTHEME_GetCurThemeInfo();
		}
		SetThemePreviewNameLabel(MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID, QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
			info_ptr, item_count, TRUE);
		SetThemePreviewSizeLabel(MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0,TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		PreQBIcon
//  Description : 	PreQBIcon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E PreQBIcon(void)
{
	uint16	 item_count = QB_THEME_ICON_PREVIEW_MAX_PAGE;
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	if (0 < s_qbtheme_anim_cur_index)
	{
		BOOLEAN is_start = FALSE;
		uint16 lcd_width = 0;
		QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_start = (BOOLEAN)(--s_qbtheme_anim_cur_index == 0);
		SetButtonGray(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, FALSE, TRUE);
		SetButtonGray(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		
		SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, (lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		if (1 < item_count)
		{
			SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, (lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		if (!is_start)
		{
			info_ptr = MMIQBTHEME_GetCurThemeInfo();
		}
		SetThemePreviewNameLabel(MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID, QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
			info_ptr, item_count, TRUE);
		SetThemePreviewSizeLabel(MMIQBTHEME_ICON_PREVIEW_GLIDE_WIN_ID, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0,TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyQBIcon
//  Description : 	ApplyQBIcon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyQBIcon(void)
{
	uint8 icon_type = THEME_DEFAULT;
	
	if (0 == s_qbtheme_anim_cur_index)
	{
		icon_type = SYSTEM_DEFAULT;
	}
	
	MMIQBTHEME_SetCurIconType(icon_type);
	MMINV_WRITE(QBTHEME_NV_MAINMENU_ICON, &icon_type);
	MMK_ReturnIdleWin();
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		HandleAnimIconGLideTpUp
//  Description : 	HandleAnimIconGLideTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimIconGLideTpUp(
								   MMI_WIN_ID_T win_id, 
								   uint16 item_max, 
								   DPARAM param
								   )
{
    int16           offset_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          i = 0;
    uint16          cur_index = 0;
    //GUI_POINT_T     tp_point = {0};
	
	offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
	
	if (!s_qbtheme_is_tp_panel
		&& 0 != offset_x
		&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
	{
        //get point
        //tp_point.x = MMK_GET_TP_X(param);
        //tp_point.y = MMK_GET_TP_Y(param);
		
		move_width = CalcMoveWidth(offset_x, item_max, QB_THEME_GLIDE_FRAME_GAP, &cur_index);
		
		//auto move
		if(0 != move_width)
		{
			for (i = 1; i <= QB_THEME_GLIDE_AUTO_FRAME; i++)
			{
				//set move space
				if (i <= (QB_THEME_GLIDE_AUTO_FRAME / 2))
				{
					move_offset = (int16)((2 * move_width * i * i) / 100);
				}
				else
				{
					move_offset = (int16)(move_width - (2 * move_width * (QB_THEME_GLIDE_AUTO_FRAME - i) * (QB_THEME_GLIDE_AUTO_FRAME - i)) / 100);
				}
				
				//set move space
				move_space = (int16)(move_offset - move_all);
				move_all   = (int16)(move_all + move_space);
				
				if (0 != move_space)
				{
					//set control rect and display rect
					SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, move_space, TRUE);
					if (1 < item_max)
					{
						SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, move_space, TRUE);
					}
					
					//update
					MMITHEME_UpdateRect();
				}
			}
		}
		//
		if (cur_index != s_qbtheme_anim_cur_index)
		{
			BOOLEAN is_start = FALSE;
			BOOLEAN is_end = FALSE;
			QBTHEME_PACKAGE_INFO_T *info_ptr = NULL;
			
			// modify title label
			s_qbtheme_anim_cur_index = cur_index;
			
			// system default
			if (0 != s_qbtheme_anim_cur_index)
			{
				info_ptr = MMIQBTHEME_GetCurThemeInfo();
			}
			
			SetThemePreviewNameLabel(win_id, QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
				info_ptr, item_max, TRUE);
			SetThemePreviewSizeLabel(win_id, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0,TRUE);
			
			is_start = 	(BOOLEAN)(0 == s_qbtheme_anim_cur_index);
			is_end = (BOOLEAN)(item_max - 1 == s_qbtheme_anim_cur_index);
			SetButtonGray(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
				IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
			SetButtonGray(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
				IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		}
		
		ResetPreviewStatus();
	}
}

/*******************************************************************/
//  Interface:		QBThemeIconPreviewInit
//  Description : 	QBThemeIconPreviewInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemeIconPreviewInit(
								  MMI_WIN_ID_T  win_id, 
								  uint16 item_count
								  )
{
	GUI_BG_T   bg_btn	= {0};
	GUI_RECT_T rc_btn_l = QB_PANEL_LEFT_BTN_RECT;
	GUI_RECT_T rc_btn_r = QB_PANEL_RIGHT_BTN_RECT;
	GUI_RECT_T rc_name = QB_PANEL_NAME_LABEL_RECT;
	GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
	GUI_RECT_T rc_key_m = QB_PANEL_MID_KEY_RECT;
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
	
	s_qbtheme_is_display_osd = TRUE;
	
	CreateOsdPanelLayer(win_id, TRUE);
	
	// left btn
	GUIBUTTON_SetRect(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, &rc_btn_l);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_LEFT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, PreQBIcon);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// right btn
	GUIBUTTON_SetRect(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, &rc_btn_r);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_RIGHT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, NextQBIcon);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// label
	bg_btn.bg_type = GUI_BG_COLOR;
	bg_btn.color = MMI_TRANSPARENCE_COLOR;
	GUILABEL_SetBg(QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, &rc_name, FALSE);
	GUILABEL_SetFont(QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	GUILABEL_SetBg(QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, &rc_size, FALSE);
	GUILABEL_SetFont(QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	if (s_qbtheme_anim_cur_index != 0)
	{
		info_ptr = MMIQBTHEME_GetCurThemeInfo();
	}
	SetThemePreviewNameLabel(win_id, QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
		info_ptr, item_count, FALSE);
	SetThemePreviewSizeLabel(win_id, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0, FALSE);
	
	// key
	GUIBUTTON_SetRect(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, &rc_key_m);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, TXT_QB_THEME_APPLY);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, ApplyQBIcon);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
	
	// preview page
	InitAnimCtrlRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, 0 - s_qbtheme_anim_cur_index);
	SetAnimCtrlParam(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, IMAGE_THEME_PREVIEW3, PNULL,TRUE, FALSE);
	if (QB_THEME_ICON_PREVIEW_MAX_PAGE == item_count)
	{		
		wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetImagePath(MMIQBTHEME_GetCurThemeInfo(), QB_IMG_PREVIEW3, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		InitAnimCtrlRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, 1 - s_qbtheme_anim_cur_index);	InitAnimCtrlRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, 1 - s_qbtheme_anim_cur_index);
		SetAnimCtrlParam(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, 0, anim_file, FALSE, FALSE);
	}
	else
	{
		MMK_DestroyControl(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID);
	}
	
	// is_start gray pre,is_end gray next
	if (0 == s_qbtheme_anim_cur_index)
	{
		
		SetButtonGray(QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, TRUE, FALSE);
	}
	if(item_count - 1 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, FALSE);
	}

    CreateReturnBtn(win_id, QBTHEME_ICON_PREVIEW_RETURN_BTN_CTRL_ID);
}

/*******************************************************************/
//  Interface:		HandleQbThemeIconPreviewGlideWinMsg
//  Description : 	HandleQbThemeIconPreviewGlideWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeIconPreviewGlideWinMsg(
													   MMI_WIN_ID_T      win_id, 	
													   MMI_MESSAGE_ID_E  msg_id,
													   DPARAM            param
													   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID;
	uint16				index		= (uint16)((uint32)MMK_GetWinAddDataPtr(win_id));
	uint16				item_count	= QB_THEME_ICON_PREVIEW_MAX_PAGE;
	
	if (PNULL == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}	
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		s_qbtheme_anim_cur_index = index;
		QBThemeIconPreviewInit(win_id, item_count);
		MMK_SetActiveCtrl(ctrl_id + index, FALSE);
		break;
	case MSG_LOSE_FOCUS:
		SetOsdLayerDisplayStatus(FALSE);
        SetRetBtnLayerDisplay(FALSE);

        if(s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
        {
            int16 offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
            
            SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, -offset_x, TRUE);
            SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, -offset_x, TRUE);
            
            ResetPreviewStatus();
        }
		break;
	case MSG_GET_FOCUS:
		SetOsdLayerDisplayStatus(TRUE);
        SetRetBtnLayerDisplay(TRUE);
		break;
	case MSG_FULL_PAINT:
		break;
	case MSG_NOTIFY_UPDATE:
		ClearOsdRectBg(win_id, *(GUI_RECT_T *)param);
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		HandleAnimGlideTpDown(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T     pt_move = {0};
			int16			offset_x = 0;
			
			//get point
			pt_move.x = MMK_GET_TP_X(param);
			pt_move.y = MMK_GET_TP_Y(param);
			
			offset_x = (int16)(pt_move.x - s_qbtheme_tp_prev_pt.x);
			if (!s_qbtheme_is_tp_panel 
				&& 0 != offset_x
				&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
			{
				GUI_RECT_T rc = MMITHEME_GetFullScreenRect();
				GUI_FillRect(MMITHEME_GetDefaultLcdDev(), rc, MMI_BACKGROUND_COLOR);
				
				SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW1_ANIM_CTRL_ID, offset_x, TRUE);
				if (PNULL != MMIQBTHEME_GetCurThemeInfo())
				{
					SetAnimCtrlDisplayRect(QBTHEME_ICON_PREVIEW2_ANIM_CTRL_ID, offset_x, TRUE);
				}
				
				s_qbtheme_tp_prev_pt.x = pt_move.x;
				s_qbtheme_tp_prev_pt.y = pt_move.y;
				s_qbtheme_has_moved = TRUE;
				
				//MMITHEME_UpdateRect();				
			}		
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
		{
			if(!s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
			{
				HandlePanelLayerTpUp(QBTHEME_ICON_PREVIEW_NAME_LABEL_CTRL_ID, QBTHEME_ICON_PREVIEW_SIZE_LABEL_CTRL_ID, \
					QBTHEME_ICON_PREVIEW_LEFT_BTN_CTRL_ID, QBTHEME_ICON_PREVIEW_RIGHT_BTN_CTRL_ID, \
					QBTHEME_ICON_PREVIEW_APPLY_BTN_CTRL_ID, 0);
			}
			else
			{
				HandleAnimIconGLideTpUp(win_id, item_count, param);
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:		
		s_qbtheme_anim_cur_index = 0;
		DestroyOsdPanelLayer();
        DestroyRetBtnLayer();
		ResetPreviewStatus();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// font preview win
/*******************************************************************/
//  Interface:		PreQBFont
//  Description : 	PreQBFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E PreQBFont(void)
{
	uint16	 item_count = QB_THEME_FONT_PREVIEW_MAX_PAGE;
	QBTHEME_PACKAGE_INFO_T* cur_info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	// if font lib not exist, -1
	if (PNULL == cur_info_ptr 
		|| 0 == MMIQBTHEME_GetPkgFontSize(cur_info_ptr))
	{
		item_count -= 1;
	}
	
	if (0 < s_qbtheme_anim_cur_index)
	{
		BOOLEAN is_start = FALSE;
		uint16 lcd_width = 0;
		QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_start = (BOOLEAN)(--s_qbtheme_anim_cur_index == 0);
		SetButtonGray(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, FALSE, TRUE);
		SetButtonGray(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		
		SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, (lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		if (1 < item_count)
		{
			SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, (lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		if (!is_start)
		{
			info_ptr = cur_info_ptr;
		}
		SetThemePreviewNameLabel(MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID, QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
			info_ptr, item_count, TRUE);
		SetThemePreviewSizeLabel(MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0,TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		NextQBFont
//  Description : 	NextQBFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E NextQBFont(void)
{
	uint16	 item_count = QB_THEME_FONT_PREVIEW_MAX_PAGE;
	QBTHEME_PACKAGE_INFO_T* cur_info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	// if font lib not exist, -1
	if (PNULL == cur_info_ptr 
		|| 0 == MMIQBTHEME_GetPkgFontSize(cur_info_ptr))
	{
		item_count -= 1;
	}
	
	if (s_qbtheme_anim_cur_index < item_count - 1)
	{
		BOOLEAN is_end = FALSE;
		uint16 lcd_width = 0;
		QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_end = (BOOLEAN)(++s_qbtheme_anim_cur_index == item_count -1);
		SetButtonGray(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
		SetButtonGray(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, FALSE, TRUE);
		
		SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, -(lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		if (1 < item_count)
		{
			SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, -(lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		if (is_end)
		{
			info_ptr = cur_info_ptr;
		}
		SetThemePreviewNameLabel(MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID, QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
			info_ptr, item_count, TRUE);
		SetThemePreviewSizeLabel(MMIQBTHEME_FONT_PREVIEW_GLIDE_WIN_ID, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0,TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyQBFont
//  Description : 	ApplyQBFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyQBFont(void)
{
	uint8 font_type = THEME_DEFAULT;
	
	if (0 == s_qbtheme_anim_cur_index)
	{
		font_type = SYSTEM_DEFAULT;
		MMIRES_UnloadFileFont();
	}
	
	MMIQBTHEME_SetCurFontType(font_type);
	MMINV_WRITE(QBTHEME_NV_FONT, &font_type);
	
	if (0 != s_qbtheme_anim_cur_index)
	{
		// load theme font
		MMIQBTHEME_LoadThemeFont(MMIQBTHEME_GetCurThemeInfo());
	}
	MMK_ReturnIdleWin();
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		QBThemeFontPreviewInit
//  Description : 	QBThemeFontPreviewInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemeFontPreviewInit(
								  MMI_WIN_ID_T  win_id, 
								  uint16 item_count
								  )
{
	GUI_BG_T   bg_btn	= {0};
	GUI_RECT_T rc_btn_l = QB_PANEL_LEFT_BTN_RECT;
	GUI_RECT_T rc_btn_r = QB_PANEL_RIGHT_BTN_RECT;
	GUI_RECT_T rc_name = QB_PANEL_NAME_LABEL_RECT;
	GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
	GUI_RECT_T rc_key_m = QB_PANEL_MID_KEY_RECT;
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;
	
	s_qbtheme_is_display_osd = TRUE;
	
	CreateOsdPanelLayer(win_id, TRUE);
	
	// left btn
	GUIBUTTON_SetRect(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, &rc_btn_l);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_LEFT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, PreQBFont);
	GUIBUTTON_SetRunSheen(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// right btn
	GUIBUTTON_SetRect(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, &rc_btn_r);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_RIGHT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, NextQBFont);
	GUIBUTTON_SetRunSheen(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// label
	bg_btn.bg_type = GUI_BG_COLOR;
	bg_btn.color = MMI_TRANSPARENCE_COLOR;
	GUILABEL_SetBg(QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, &rc_name, FALSE);
	GUILABEL_SetFont(QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	GUILABEL_SetBg(QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, &rc_size, FALSE);
	GUILABEL_SetFont(QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	if (s_qbtheme_anim_cur_index != 0)
	{
		info_ptr = MMIQBTHEME_GetCurThemeInfo();
	}
	SetThemePreviewNameLabel(win_id, QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
		info_ptr, item_count, FALSE);
	SetThemePreviewSizeLabel(win_id, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, \
		MMIQBTHEME_GetPkgFontSize(info_ptr), FALSE);
	
	// key
	GUIBUTTON_SetRect(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, &rc_key_m);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, TXT_QB_THEME_APPLY);
	GUIBUTTON_SetCallBackFunc(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, ApplyQBFont);
	GUIBUTTON_SetRunSheen(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
	
	// preview page
	InitAnimCtrlRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, 0 - s_qbtheme_anim_cur_index);	
	SetAnimCtrlParam(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, IMAGE_THEME_FONT_SAMPLE, PNULL, TRUE, FALSE);
	
	if (QB_THEME_FONT_PREVIEW_MAX_PAGE == item_count)
	{		
		wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		MMIQBTHEME_GetImagePath(MMIQBTHEME_GetCurThemeInfo(), QB_IMG_FONT_SAMPLE, anim_file, MMIFILE_FULL_PATH_MAX_LEN);
		InitAnimCtrlRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, 1 - s_qbtheme_anim_cur_index);
		SetAnimCtrlParam(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, 0, anim_file, FALSE, FALSE);
	}
	else
	{
		MMK_DestroyControl(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID);
	}
	
	// is_start gray pre,is_end gray next
	if (0 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, TRUE, FALSE);
	}
	if(item_count - 1 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, FALSE);
	}

    CreateReturnBtn(win_id, QBTHEME_FONT_PREVIEW_RETURN_BTN_CTRL_ID);
}

/*******************************************************************/
//  Interface:		HandleAnimFontGLideTpUp
//  Description : 	HandleAnimFontGLideTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimFontGLideTpUp(
								   MMI_WIN_ID_T win_id, 
								   uint16 item_max, 
								   DPARAM param
								   )
{
    int16           offset_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          i = 0;
    uint16          cur_index = 0;
    //GUI_POINT_T     tp_point = {0};
	
	offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
	
	if (!s_qbtheme_is_tp_panel
		&& 0 != offset_x
		&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
	{
        //get point
        //tp_point.x = MMK_GET_TP_X(param);
        //tp_point.y = MMK_GET_TP_Y(param);
		
		move_width = CalcMoveWidth(offset_x, item_max, QB_THEME_GLIDE_FRAME_GAP, &cur_index);
		
		//auto move
		if(0 != move_width)
		{
			for (i = 1; i <= QB_THEME_GLIDE_AUTO_FRAME; i++)
			{
				//set move space
				if (i <= (QB_THEME_GLIDE_AUTO_FRAME / 2))
				{
					move_offset = (int16)((2 * move_width * i * i) / 100);
				}
				else
				{
					move_offset = (int16)(move_width - (2 * move_width * (QB_THEME_GLIDE_AUTO_FRAME - i) * (QB_THEME_GLIDE_AUTO_FRAME - i)) / 100);
				}
				
				//set move space
				move_space = (int16)(move_offset - move_all);
				move_all   = (int16)(move_all + move_space);
				
				if (0 != move_space)
				{
					//set control rect and display rect
					SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, move_space, TRUE);
					if (QB_THEME_FONT_PREVIEW_MAX_PAGE == item_max)
					{
						SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, move_space, TRUE);
					}
					
					//update
					MMITHEME_UpdateRect();
				}
			}
		}
		//
		if (cur_index != s_qbtheme_anim_cur_index)
		{
			BOOLEAN is_start = FALSE;
			BOOLEAN is_end = FALSE;
			QBTHEME_PACKAGE_INFO_T *info_ptr = NULL;
			
			// modify title label
			s_qbtheme_anim_cur_index = cur_index;
			
			// system default
			if (0 != s_qbtheme_anim_cur_index)
			{
				info_ptr = MMIQBTHEME_GetCurThemeInfo();
			}
			
			SetThemePreviewNameLabel(win_id, QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
				info_ptr, item_max, TRUE);
			SetThemePreviewSizeLabel(win_id, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, \
				MMIQBTHEME_GetPkgFontSize(info_ptr),TRUE);
			
			is_start = 	(BOOLEAN)(0 == s_qbtheme_anim_cur_index);
			is_end = (BOOLEAN)(item_max - 1 == s_qbtheme_anim_cur_index);
			SetButtonGray(QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
				IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
			SetButtonGray(QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
				IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		}
		
		ResetPreviewStatus();
	}
}

/*******************************************************************/
//  Interface:		HandleQbThemeFontPreviewGlideWinMsg
//  Description : 	HandleQbThemeFontPreviewGlideWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeFontPreviewGlideWinMsg(
													   MMI_WIN_ID_T      win_id, 	
													   MMI_MESSAGE_ID_E  msg_id,
													   DPARAM            param
													   )
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID;
	uint16				index		= (uint16)((uint32)MMK_GetWinAddDataPtr(win_id));
	uint16				item_count	= QB_THEME_FONT_PREVIEW_MAX_PAGE;
	QBTHEME_PACKAGE_INFO_T* info_ptr = MMIQBTHEME_GetCurThemeInfo();

	// if font lib not exist, -1
	if (PNULL == info_ptr 
		|| 0 == MMIQBTHEME_GetPkgFontSize(info_ptr))
	{
		item_count -= 1;
	}	
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		s_qbtheme_anim_cur_index = index;
		QBThemeFontPreviewInit(win_id, item_count);
		MMK_SetActiveCtrl(ctrl_id + index, FALSE);
		break;
	case MSG_LOSE_FOCUS:
		SetOsdLayerDisplayStatus(FALSE);
        SetRetBtnLayerDisplay(FALSE);

        if(s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
        {
            int16 offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
            
            SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, -offset_x, TRUE);
            SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, -offset_x, TRUE);
            
            ResetPreviewStatus();
        }
		break;
	case MSG_GET_FOCUS:
		SetOsdLayerDisplayStatus(TRUE);
        SetRetBtnLayerDisplay(TRUE);
		break;
	case MSG_FULL_PAINT:
		break;
	case MSG_NOTIFY_UPDATE:
		ClearOsdRectBg(win_id, *(GUI_RECT_T *)param);
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		HandleAnimGlideTpDown(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T     pt_move = {0};
			int16			offset_x = 0;
			
			//get point
			pt_move.x = MMK_GET_TP_X(param);
			pt_move.y = MMK_GET_TP_Y(param);
			
			offset_x = (int16)(pt_move.x - s_qbtheme_tp_prev_pt.x);
			if (!s_qbtheme_is_tp_panel 
				&& 0 != offset_x
				&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
			{
				GUI_RECT_T rc = MMITHEME_GetFullScreenRect();
				GUI_FillRect(MMITHEME_GetDefaultLcdDev(), rc, MMI_BACKGROUND_COLOR);
				
				SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW1_ANIM_CTRL_ID, offset_x, TRUE);
				if (QB_THEME_FONT_PREVIEW_MAX_PAGE == item_count)
				{
					SetAnimCtrlDisplayRect(QBTHEME_FONT_PREVIEW2_ANIM_CTRL_ID, offset_x, TRUE);
				}
				
				s_qbtheme_tp_prev_pt.x = pt_move.x;
				s_qbtheme_tp_prev_pt.y = pt_move.y;
				s_qbtheme_has_moved = TRUE;
				
				//MMITHEME_UpdateRect();				
			}		
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
		{
			if(!s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
			{
				HandlePanelLayerTpUp(QBTHEME_FONT_PREVIEW_NAME_LABEL_CTRL_ID, QBTHEME_FONT_PREVIEW_SIZE_LABEL_CTRL_ID, \
					QBTHEME_FONT_PREVIEW_LEFT_BTN_CTRL_ID, QBTHEME_FONT_PREVIEW_RIGHT_BTN_CTRL_ID, \
					QBTHEME_FONT_PREVIEW_APPLY_BTN_CTRL_ID, 0);
			}
			else
			{
				HandleAnimFontGLideTpUp(win_id, item_count, param);
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:		
		s_qbtheme_anim_cur_index = 0;
		DestroyOsdPanelLayer();
        DestroyRetBtnLayer();
		ResetPreviewStatus();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
													   }

//////////////////////////////////////////////////////////////////////////
// icon bg preview win

/*******************************************************************/
//  Interface:		SetIconLayerDisplayStatus
//  Description : 	SetIconLayerDisplayStatus
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void SetIconLayerDisplayStatus(
									 GUI_LCD_DEV_INFO const *layer_ptr, 
									 BOOLEAN is_display
									 )
{
	UILAYER_APPEND_BLT_T append_info = {0};
	
	if ((0 != layer_ptr->block_id)
		&& UILAYER_IsLayerActive(layer_ptr))
    {
		append_info.layer_level = UILAYER_LEVEL_NORMAL;
		append_info.lcd_dev_info = *layer_ptr;
		
		if (is_display)
		{
			UILAYER_AppendBltLayer(&append_info);		
		}
		else
		{
			UILAYER_RemoveBltLayer(&append_info);
		}
	}
}

/*******************************************************************/
//  Interface:		DestroyIconLayer
//  Description : 	DestroyIconLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyIconLayer(
							GUI_LCD_DEV_INFO *layer_ptr
							)
{
	UILAYER_ReleaseLayer(layer_ptr);
	layer_ptr->block_id = UILAYER_NULL_HANDLE;
}

/*******************************************************************/
//  Interface:		CreateIconLayer
//  Description : 	CreateIconLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CreateIconLayer(
						   MMI_WIN_ID_T win_id, 
						   BOOLEAN is_append
						   )
{
	UILAYER_CREATE_T    create_info = {0};
	uint16				lcd_width = 0;
	uint16				lcd_height = 0;
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
	
	if (UILAYER_IsMultiLayerEnable())
	{
		// icon
		DestroyIconLayer(&s_qbtheme_icon_layer);
		
        //create
		create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        UILAYER_CreateLayer(&create_info, &s_qbtheme_icon_layer);
		
		//set color key
        UILAYER_SetLayerColorKey(&s_qbtheme_icon_layer, TRUE,UILAYER_TRANSPARENT_COLOR);
		UILAYER_Clear(&s_qbtheme_icon_layer);
		
		//append layer
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, is_append);
	}
}

/*******************************************************************/
//  Interface:		CreateIconBgLayer
//  Description : 	CreateIconBgLayer
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CreateIconBgLayer(
							 MMI_WIN_ID_T	win_id,
							 uint16		max_page, 
							 BOOLEAN	is_append
							 )
{
	UILAYER_CREATE_T    create_info = {0};
	uint16				lcd_width = 0;
	uint16				lcd_height = 0;
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
	if (lcd_height < lcd_width)
	{
		return;
	}
	
	if (UILAYER_IsMultiLayerEnable())
    {
		uint16 start_h = (lcd_height - lcd_width)/2 - 1;
		
		// icon bg
		DestroyIconLayer(&s_qbtheme_icon_bg_layer);
		
		// create
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = start_h;
// #ifndef RUNTIME_DRAW_SUPPORT
//         create_info.width = (int16)(lcd_width * max_page);
// #else
		create_info.width = (int16)lcd_width;
// #endif
        create_info.height = lcd_width;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        UILAYER_CreateLayer(&create_info, &s_qbtheme_icon_bg_layer);
		
        // set color key
		UILAYER_SetLayerColorKey(&s_qbtheme_icon_bg_layer, TRUE,UILAYER_TRANSPARENT_COLOR);
		UILAYER_Clear(&s_qbtheme_icon_bg_layer);
		
		// append
		SetIconLayerDisplayStatus(&s_qbtheme_icon_bg_layer, is_append);
	}
}

/*******************************************************************/
//  Interface:		GetMenuIconRect
//  Description : 	GetMenuIconRect
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL GUI_RECT_T GetMenuIconRect(
								 uint16 n_row, 
								 uint16 n_cloumn, 
								 int16 n_page
								 )
{
	uint16				lcd_width = 0;
	uint16				lcd_height = 0;
	uint16              item_h = 0;
	uint16				item_w = 0;
	uint16              start_h = 0;
	GUI_RECT_T			rc_item = {0};
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
	
	if (lcd_height > lcd_width)
	{
		item_w = lcd_width / QB_THEME_ICON_BG_ICON_COLUMN;
		item_h = item_w;
		start_h = (lcd_height - item_h * QB_THEME_ICON_BG_ICON_ROW) / 2 - 1;
		
		rc_item.left = (int16)(n_cloumn *  item_w + n_page * lcd_width);
		rc_item.right = rc_item.left + item_w - 1;
		rc_item.top = start_h + n_row * item_h;
		rc_item.bottom = rc_item.top + item_h -1;
	}
	
	return rc_item;
}

/*lint -e831 -e661*/
/*******************************************************************/
//  Interface:		DisplayMenuIcon
//  Description : 	DisplayMenuIcon
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DisplayMenuIcon(
						   MMI_WIN_ID_T win_id,
						   GUI_LCD_DEV_INFO const *layer_ptr
						   )
{
	GUI_RECT_T rc_item = {0};
    uint16              img_width = 0;
    uint16              img_height = 0;
	uint16 i = 0;
	uint16 j = 0;
	BOOLEAN is_need = MMIAPIQBTHEME_GetNeedTransform();

	if (!s_qbtheme_is_show_icon)
	{
		return;
	}

	GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_MAINMENU_ICON_PB, win_id);
	
	MMIQBTHEME_SetNeedTransform(TRUE);
	for (i = 0; i < QB_THEME_ICON_BG_ICON_ROW; i++ )
	{
		for(j = 0; j < QB_THEME_ICON_BG_ICON_COLUMN; j++)
		{
			rc_item = GetMenuIconRect(i, j, 0);
			rc_item = GUI_GetCenterRect(rc_item, img_width, img_height);
			GUIRES_DisplayImg(PNULL, &rc_item, PNULL, win_id, \
                s_qbtheme_menu_icons[i * QB_THEME_ICON_BG_ICON_COLUMN + j], layer_ptr);
		}
	}
	MMIQBTHEME_SetNeedTransform(is_need);
}
/*lint -e831 -e661*/

/*******************************************************************/
//  Interface:		DisplayMenuIconBg
//  Description : 	DisplayMenuIconBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DisplayMenuIconBg(
							 MMI_WIN_ID_T win_id
							 )
{
	GUI_RECT_T	rc_item = {0};
	uint16	img_width = 0;
    uint16	img_height = 0;
	uint16	i = 0;
	uint16	j = 0;
	
	GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_THEME_MAINMENU_ICON_BG, win_id);
	
	for (i = 0; i < QB_THEME_ICON_BG_ICON_ROW; i++ )
	{
		for(j = 0; j < QB_THEME_ICON_BG_ICON_COLUMN; j++)
		{
			rc_item = GetMenuIconRect(i, j, 0);
			rc_item = GUI_GetCenterRect(rc_item, img_width, img_height);
			
			MMIQBTHEME_SetNeedTransform(FALSE);
			GUIRES_DisplayImg(PNULL, &rc_item, PNULL, win_id, IMAGE_THEME_MAINMENU_ICON_BG, &s_qbtheme_icon_bg_layer);
			MMIQBTHEME_SetNeedTransform(TRUE);
			
			if(0 != MMIQBTHEME_GetCurThemeInfo())
			{
				rc_item = GetMenuIconRect(i, j, 2);
				rc_item = GUI_GetCenterRect(rc_item, img_width, img_height);
				
				MMIQBTHEME_SetForceTransform(TRUE);
				GUIRES_DisplayImg(PNULL, &rc_item, PNULL, win_id, IMAGE_THEME_MAINMENU_ICON_BG, &s_qbtheme_icon_bg_layer);
				MMIQBTHEME_SetForceTransform(FALSE);
			}
		}
	}	
}

/*******************************************************************/
//  Interface:		IconBgSetUpLayerLable
//  Description : 	IconBgSetUpLayerLable
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void IconBgSetUpLayerLable(
								 MMI_WIN_ID_T win_id, 
								 uint16		item_max,
								 BOOLEAN	is_update
								 )
{
	MMI_TEXT_ID_T  name_id = 0;
	QBTHEME_PACKAGE_INFO_T *info_ptr = PNULL;

	if (QB_ICON_BG_SYSTEM == s_qbtheme_anim_cur_index)
	{
		name_id = TXT_COMMON_CLASSIC;
	}
	else if (QB_ICON_BG_TRANSPARENT == s_qbtheme_anim_cur_index)
	{
		name_id = TXT_QB_TRANSPARENT;
	}
	if (QB_ICON_BG_THEME == s_qbtheme_anim_cur_index)
	{
		info_ptr = MMIQBTHEME_GetCurThemeInfo();
	}
	
	SetThemePreviewNameLabel(win_id, QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID, name_id, PNULL, info_ptr, item_max, is_update);
	SetThemePreviewSizeLabel(win_id, QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr, 0, is_update);			
}

/*******************************************************************/
//  Interface:		NextQBIconBg
//  Description : 	NextQBIconBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E NextQBIconBg(void)
{
	uint16	item_count = QB_THEME_ICON_BG_PREVIEW_MAX_PAGE;
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	if (s_qbtheme_anim_cur_index < item_count - 1)
	{
		BOOLEAN is_end = FALSE;
		uint16 lcd_width = 0;
		int16 layer_x = 0;
		int16 layer_y = 0;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_end = (BOOLEAN)(++s_qbtheme_anim_cur_index == item_count -1);
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, FALSE, TRUE);

// #ifndef RUNTIME_DRAW_SUPPORT		
// 		UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
// 		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x - lcd_width, layer_y);
// #else
		UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
		UILAYER_Clear(&s_qbtheme_icon_bg_layer);
		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + (lcd_width * s_qbtheme_anim_cur_index), layer_y);
		
		DisplayMenuIconBg(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID);
		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
		DisplayMenuIcon(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID, &s_qbtheme_icon_bg_layer);
// #endif
		
		IconBgSetUpLayerLable(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID, item_count, TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		PreQBIconBg
//  Description : 	PreQBIconBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E PreQBIconBg(void)
{
	uint16	item_count = QB_THEME_ICON_BG_PREVIEW_MAX_PAGE;
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	if (0 < s_qbtheme_anim_cur_index)
	{
		BOOLEAN is_start = FALSE;
		uint16 lcd_width = 0;
		int16 layer_x = 0;
		int16 layer_y = 0;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_start = (BOOLEAN)(--s_qbtheme_anim_cur_index == 0);
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, FALSE, TRUE);
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);

// #ifndef RUNTIME_DRAW_SUPPORT		
// 		UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
// 		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + lcd_width, layer_y);
// #else
		UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
		UILAYER_Clear(&s_qbtheme_icon_bg_layer);
		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + (lcd_width * s_qbtheme_anim_cur_index), layer_y);
		
		DisplayMenuIconBg(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID);
		UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
		DisplayMenuIcon(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID, &s_qbtheme_icon_bg_layer);
// #endif
		
		IconBgSetUpLayerLable(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID, item_count, TRUE);
		
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyQBIconBg
//  Description : 	ApplyQBIconBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyQBIconBg(void)
{
	uint8 icon_type = THEME_DEFAULT;
	
	if (0 == s_qbtheme_anim_cur_index)
	{
		icon_type = SYSTEM_DEFAULT;
	}
	else if (1 == s_qbtheme_anim_cur_index)
	{
		icon_type = DEFAULT_NONE;
	}
	
	MMIQBTHEME_SetCurIconBgType(icon_type);
	MMINV_WRITE(QBTHEME_NV_ICON_BG, &icon_type);
	MMK_ReturnIdleWin();
	
	return MMI_RESULT_TRUE;	
}

/*******************************************************************/
//  Interface:		SetDispQBIconBg
//  Description : 	SetDispQBIconBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetDispQBIconBg(void)
{
	MMI_CTRL_ID_T ctrl_id = QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID;
	BOOLEAN is_active = FALSE;
	
// #ifndef RUNTIME_DRAW_SUPPORT
// 	is_active = UILAYER_IsBltLayer(&s_qbtheme_icon_layer);
// #else
	uint16 lcd_width = 0;
	int16 layer_x = 0;
	int16 layer_y = 0;
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);

	is_active = s_qbtheme_is_show_icon;
	s_qbtheme_is_show_icon = !s_qbtheme_is_show_icon;
	
	UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
	UILAYER_Clear(&s_qbtheme_icon_bg_layer);
	UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + (lcd_width * s_qbtheme_anim_cur_index), layer_y);
	
	DisplayMenuIconBg(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID);
	UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
	DisplayMenuIcon(MMIQBTHEME_ICON_BG_PREVIEW_GLIDE_WIN_ID, &s_qbtheme_icon_bg_layer);
//#endif
	
	if (is_active)
	{
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, FALSE);
		GUIBUTTON_SetTextId(ctrl_id, TXT_QB_SHOW_ICONS);
	}
	else
	{
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, TRUE);
		GUIBUTTON_SetTextId(ctrl_id, TXT_QB_HIDE_ICONS);	
	}
	
	GUIBUTTON_Update(ctrl_id);
	MMITHEME_UpdateRect();
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		QBThemeIconBgDisplayBg
//  Description : 	QBThemeIconBgDisplayBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemeIconBgDisplayBg(
								  MMI_WIN_ID_T  win_id
								  )
{
	GUI_POINT_T pt = {0};
	GUI_RECT_T rc = MMITHEME_GetFullScreenRect();
	
	GUIRES_DisplayImg(&pt, PNULL, &rc, win_id, IMAGE_THEME_BG, MMITHEME_GetDefaultLcdDev());
}

/*******************************************************************/
//  Interface:		HandleAnimIconBgGLideTpUp
//  Description : 	HandleAnimIconBgGLideTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimIconBgGLideTpUp(
									 MMI_WIN_ID_T win_id, 
									 DPARAM param
									 )
{
    int16           offset_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          lcd_width = 0;
    uint16          i = 0;
    uint16          cur_index = 0;
	uint16			item_max = QB_THEME_ICON_BG_PREVIEW_MAX_PAGE;
    //GUI_POINT_T     tp_point = {0};
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_max -= 1;
	}
	offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
	
	if (!s_qbtheme_is_tp_panel
		&& 0 != offset_x
		&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
	{
        //get point
        //tp_point.x = MMK_GET_TP_X(param);
        //tp_point.y = MMK_GET_TP_Y(param);
		
        //get lcd width
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		//set start index
        cur_index = s_qbtheme_anim_cur_index;
		
		move_width = CalcMoveWidth(offset_x, item_max, 0, &cur_index);
		
		//auto move
		if(0 != move_width)
		{
			for (i = 1; i <= QB_THEME_GLIDE_AUTO_FRAME; i++)
			{
				//set move space
				if (i <= (QB_THEME_GLIDE_AUTO_FRAME / 2))
				{
					move_offset = (int16)((2 * move_width * i * i) / 100);
				}
				else
				{
					move_offset = (int16)(move_width - (2 * move_width * (QB_THEME_GLIDE_AUTO_FRAME - i) * (QB_THEME_GLIDE_AUTO_FRAME - i)) / 100);
				}
				
				//set move space
				move_space = (int16)(move_offset - move_all);
				move_all   = (int16)(move_all + move_space);
				
				if (0 != move_space)
				{
					int16 layer_x = 0;
					int16 layer_y = 0;
					
// #ifndef RUNTIME_DRAW_SUPPORT					
// 					UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
// 					UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + move_space, layer_y);
// #else
					UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
					UILAYER_Clear(&s_qbtheme_icon_bg_layer);
					UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x - offset_x - move_all + (lcd_width * s_qbtheme_anim_cur_index) , layer_y);
					
					DisplayMenuIconBg(win_id);
					UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
					DisplayMenuIcon(win_id, &s_qbtheme_icon_bg_layer);
//#endif
					
					//update
					MMITHEME_UpdateRect();
				}
			}
		}
		//
		if (cur_index != s_qbtheme_anim_cur_index)
		{
			BOOLEAN is_start = FALSE;
			BOOLEAN is_end = FALSE;
			// modify title label
			s_qbtheme_anim_cur_index = cur_index;
			IconBgSetUpLayerLable(win_id, item_max, TRUE);
			
			is_start = 	(BOOLEAN)(0 == s_qbtheme_anim_cur_index);
			is_end = (BOOLEAN)(item_max - 1 == s_qbtheme_anim_cur_index);
			SetButtonGray(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
				IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
			SetButtonGray(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
				IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		}
		
		ResetPreviewStatus();
	}
}

/*******************************************************************/
//  Interface:		QBThemeIconBgPreviewInit
//  Description : 	QBThemeIconBgPreviewInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemeIconBgPreviewInit(
									MMI_WIN_ID_T  win_id, 
									uint16 item_count
									)
{
	GUI_BG_T   bg_btn	= {0};
	GUI_RECT_T rc_btn_l = QB_PANEL_LEFT_BTN_RECT;
	GUI_RECT_T rc_btn_r = QB_PANEL_RIGHT_BTN_RECT;
	GUI_RECT_T rc_name = QB_PANEL_NAME_LABEL_RECT;
	GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
	GUI_RECT_T rc_key_l = QB_PANEL_LEFT_KEY_RECT;
	GUI_RECT_T rc_key_r = QB_PANEL_RIGHT_KEY_RECT;
	
	uint16 lcd_width = 0;
	int16 layer_x = 0;
	int16 layer_y = 0;
	
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
	s_qbtheme_is_display_osd = TRUE;
	
	// menu
	CreateIconBgLayer(win_id, item_count, TRUE);

// #ifndef RUNTIME_DRAW_SUPPORT
// 	DisplayMenuIconBg(win_id);	
// 	// move
// 	UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
// 	UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x - (lcd_width * s_qbtheme_anim_cur_index), layer_y);
// 
// 	CreateIconLayer(win_id, TRUE);
// 	DisplayMenuIcon(win_id, &s_qbtheme_icon_layer);
// #else
	// move
	UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
	UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + (lcd_width * s_qbtheme_anim_cur_index), layer_y);
	
	DisplayMenuIconBg(win_id);
	UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
	DisplayMenuIcon(win_id, &s_qbtheme_icon_bg_layer);
// #endif
	
	// osd
	CreateOsdPanelLayer(win_id, TRUE);
	
	// left btn
	GUIBUTTON_SetRect(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, &rc_btn_l);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_LEFT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, PreQBIconBg);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// right btn
	GUIBUTTON_SetRect(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, &rc_btn_r);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_RIGHT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, NextQBIconBg);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// label
	bg_btn.bg_type = GUI_BG_COLOR;
	bg_btn.color = MMI_TRANSPARENCE_COLOR;
	GUILABEL_SetBg(QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID, &rc_name, FALSE);
	GUILABEL_SetFont(QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	GUILABEL_SetBg(QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID, &rc_size, FALSE);
	GUILABEL_SetFont(QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	IconBgSetUpLayerLable(win_id, item_count, FALSE);
	
	// key left
	GUIBUTTON_SetRect(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, &rc_key_l);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, TXT_QB_HIDE_ICONS);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, SetDispQBIconBg);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
	
	// key right
	GUIBUTTON_SetRect(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, &rc_key_r);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, TXT_QB_THEME_APPLY);
	GUIBUTTON_SetCallBackFunc(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, ApplyQBIconBg);
	GUIBUTTON_SetRunSheen(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
	
	// preview page
	//QBThemeIconBgDisplayBg(win_id);
	
	// is_start gray pre,is_end gray next
	if (0 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, TRUE, FALSE);
	}
	if(item_count - 1 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, FALSE);
	}

    CreateReturnBtn(win_id, QBTHEME_ICON_BG_PREVIEW_RETURN_BTN_CTRL_ID);
}

/*******************************************************************/
//  Interface:		HandleQbThemeIconBgPreviewGlideWinMsg
//  Description : 	HandleQbThemeIconBgPreviewGlideWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeIconBgPreviewGlideWinMsg(
														 MMI_WIN_ID_T      win_id, 	
														 MMI_MESSAGE_ID_E  msg_id,
														 DPARAM            param
														 )
{
	MMI_RESULT_E	recode	= MMI_RESULT_TRUE;
	MMI_CTRL_ID_T	ctrl_id	= QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID;
	uint16	index  = (uint16)((uint32)MMK_GetWinAddDataPtr(win_id));
	uint32	item_count = QB_THEME_ICON_BG_PREVIEW_MAX_PAGE;
	
	if (0 == MMIQBTHEME_GetCurThemeInfo())
	{
		item_count -= 1;
	}
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		s_qbtheme_anim_cur_index = index;
		QBThemeIconBgPreviewInit(win_id, item_count);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
		break;
	case MSG_LOSE_FOCUS:
		SetOsdLayerDisplayStatus(FALSE);
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer,FALSE);
		SetIconLayerDisplayStatus(&s_qbtheme_icon_bg_layer,FALSE);
        SetRetBtnLayerDisplay(FALSE);

        if(s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
        {
            int16 layer_x = 0;
            int16 layer_y = 0;
            int16 offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
            
            UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
            UILAYER_Clear(&s_qbtheme_icon_bg_layer);
            UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x - offset_x, layer_y);
            
            DisplayMenuIconBg(win_id);
            UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
            DisplayMenuIcon(win_id, &s_qbtheme_icon_bg_layer);
            
            ResetPreviewStatus();
        }
		break;
	case MSG_GET_FOCUS:
		SetIconLayerDisplayStatus(&s_qbtheme_icon_bg_layer,TRUE);
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer,TRUE);
		SetOsdLayerDisplayStatus(TRUE);
        SetRetBtnLayerDisplay(TRUE);
// #ifndef RUNTIME_DRAW_SUPPORT
// 		GUIBUTTON_SetTextId(QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID, TXT_QB_HIDE_ICONS);
// #endif
		break;
	case MSG_FULL_PAINT:
		QBThemeIconBgDisplayBg(win_id);
		break;
	case MSG_NOTIFY_UPDATE:
		ClearOsdRectBg(win_id, *(GUI_RECT_T *)param);
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		HandleAnimGlideTpDown(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T     pt_move = {0};
			int16			offset_x = 0;	
			
			//get point
			pt_move.x = MMK_GET_TP_X(param);
			pt_move.y = MMK_GET_TP_Y(param);

// #ifndef RUNTIME_DRAW_SUPPORT			
// 			offset_x = (int16)(pt_move.x - s_qbtheme_tp_prev_pt.x);
// #else	
			offset_x = (int16)(pt_move.x - s_qbtheme_tp_start_pt.x);
// #endif
			if (!s_qbtheme_is_tp_panel 
				&& 0 != offset_x
				&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
			{
				int16 layer_x = 0;
				int16 layer_y = 0;
				
// #ifndef RUNTIME_DRAW_SUPPORT				
// 				UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
// 				UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x + offset_x, layer_y);
// #else
				uint16			lcd_width = 0;
				GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
				
				UILAYER_GetLayerPosition(&s_qbtheme_icon_bg_layer, &layer_x, &layer_y);
				UILAYER_Clear(&s_qbtheme_icon_bg_layer);
				UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x - offset_x + (lcd_width * s_qbtheme_anim_cur_index), layer_y);
				
				DisplayMenuIconBg(win_id);
				UILAYER_SetLayerPosition(&s_qbtheme_icon_bg_layer, layer_x, layer_y);
				DisplayMenuIcon(win_id, &s_qbtheme_icon_bg_layer);
// #endif
				
				if (UILAYER_IsBltLayer(&s_qbtheme_icon_layer))
				{
					SetOsdLayerDisplayStatus(FALSE);
					CopyOsdPanelLayer(&s_qbtheme_icon_layer, FALSE);
				}
				
				s_qbtheme_tp_prev_pt.x = pt_move.x;
				s_qbtheme_tp_prev_pt.y = pt_move.y;
				s_qbtheme_has_moved = TRUE;
				
				//MMITHEME_UpdateRect();				
			}
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
		{
			if(!s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
			{
				HandlePanelLayerTpUp(QBTHEME_ICON_BG_PREVIEW_NAME_LABEL_CTRL_ID, QBTHEME_ICON_BG_PREVIEW_SIZE_LABEL_CTRL_ID, \
					QBTHEME_ICON_BG_PREVIEW_LEFT_BTN_CTRL_ID, QBTHEME_ICON_BG_PREVIEW_RIGHT_BTN_CTRL_ID, \
					QBTHEME_ICON_BG_PREVIEW_APPLY_BTN_CTRL_ID, QBTHEME_ICON_BG_PREVIEW_DISP_BTN_CTRL_ID);
			}
			else
			{
				HandleAnimIconBgGLideTpUp(win_id, param);
				if (UILAYER_IsBltLayer(&s_qbtheme_icon_layer))
				{
					ClearOsdPanelLayer(&s_qbtheme_icon_layer);
					SetOsdLayerDisplayStatus(TRUE);
				}
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:		
		s_qbtheme_anim_cur_index = 0;
		s_qbtheme_is_show_icon = TRUE;
		DestroyIconLayer(&s_qbtheme_icon_layer);
		DestroyIconLayer(&s_qbtheme_icon_bg_layer);
		DestroyOsdPanelLayer();
        DestroyRetBtnLayer();
		ResetPreviewStatus();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
// wallpaper preview
/*******************************************************************/
//  Interface:		NextQBWallpaper
//  Description : 	NextQBWallpaper
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E NextQBWallpaper(void)
{
	uint32 i = 0;
	uint32  item_count  = MMIQBTHEME_GetThemeWallpaperCount() + 1;
	
	if (item_count > QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE)
	{
		item_count = QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE;
	}
	
	if (s_qbtheme_anim_cur_index < (uint16)item_count - 1)
	{
		BOOLEAN is_end = FALSE;
		uint16 lcd_width = 0;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_end = (BOOLEAN)(++s_qbtheme_anim_cur_index == (uint16)item_count -1);
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, FALSE, TRUE);
		
		for (; i < item_count; i++)
		{
			SetAnimCtrlDisplayRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + i, -(lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		
		WallpaperSetUpLayerLable(MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID, MMIQBTHEME_GetCurThemeInfo(), (uint16)item_count, TRUE);
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		PreQBWallpaper
//  Description : 	PreQBWallpaper
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E PreQBWallpaper(void)
{
	uint32 i = 0;
	uint32  item_count  = MMIQBTHEME_GetThemeWallpaperCount() + 1;
	
	if (item_count > QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE)
	{
		item_count = QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE;
	}
	
	if (0 < s_qbtheme_anim_cur_index)
	{
		BOOLEAN is_start = FALSE;
		uint16 lcd_width = 0;
		
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
		
		is_start = (BOOLEAN)(--s_qbtheme_anim_cur_index == 0);
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, FALSE, TRUE);
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		
		for (; i < item_count; i++)
		{
			SetAnimCtrlDisplayRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + i, (lcd_width + QB_THEME_GLIDE_FRAME_GAP), TRUE);
		}
		
		WallpaperSetUpLayerLable(MMIQBTHEME_WALLPAPER_PREVIEW_GLIDE_WIN_ID, MMIQBTHEME_GetCurThemeInfo(), (uint16)item_count, TRUE);
		//update
		MMITHEME_UpdateRect();	
	}
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		ApplyQBWallpaper
//  Description : 	ApplyQBWallpaper
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E ApplyQBWallpaper(void)
{
	wchar* wallpaper_ptr = MMIQBTHEME_GetCurWallpaper();

	if (PNULL != wallpaper_ptr)
	{
		SCI_MEMSET(wallpaper_ptr, 0, sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
		if (0 != s_qbtheme_anim_cur_index)
		{
			wchar *new_wallpaper_ptr = MMIQBTHEME_GetWallpaperName(s_qbtheme_anim_cur_index - 1);
			MMIAPICOM_Wstrcpy(wallpaper_ptr, new_wallpaper_ptr);
		}
		
		MMINV_WRITE(QBTHEME_NV_WALLPAPER, wallpaper_ptr);
		
		//
		ResetWallpaperSet();
		
		//MMI_FreeRes(MAIN_IDLE_WIN_ID);
		
		MMK_ReturnIdleWin();
	}
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		SetDispQBWallpaper
//  Description : 	SetDispQBWallpaper
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E SetDispQBWallpaper(void)
{
	MMI_CTRL_ID_T ctrl_id = QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID;
	BOOLEAN is_active = UILAYER_IsBltLayer(&s_qbtheme_icon_layer);
	
	if (is_active)
	{
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, FALSE);
		GUIBUTTON_SetTextId(ctrl_id, TXT_QB_SHOW_ICONS);
	}
	else
	{
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, TRUE);
		GUIBUTTON_SetTextId(ctrl_id, TXT_QB_HIDE_ICONS);	
	}
	
	GUIBUTTON_Update(ctrl_id);
	MMITHEME_UpdateRect();
	
	return MMI_RESULT_TRUE;
}

/*******************************************************************/
//  Interface:		WallpaperSetUpLayerLable
//  Description : 	WallpaperSetUpLayerLable
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void WallpaperSetUpLayerLable(
									MMI_WIN_ID_T win_id, 
									QBTHEME_PACKAGE_INFO_T *info_ptr, 
									uint16 item_max,
									BOOLEAN is_update
									)
{
	if (0 == s_qbtheme_anim_cur_index)
	{
		SetThemePreviewNameLabel(win_id, QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, 0, PNULL,\
			PNULL, item_max, is_update);
		SetThemePreviewSizeLabel(win_id, QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, PNULL, 0, is_update);
	}
	else
	{
		wchar *wallpaper_ptr = MMIQBTHEME_GetWallpaperName(s_qbtheme_anim_cur_index - 1);
		
		if (PNULL != wallpaper_ptr)
		{
			SetThemePreviewNameLabel(win_id, QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, 0, wallpaper_ptr,\
				info_ptr, item_max, is_update);
			SetThemePreviewSizeLabel(win_id, QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, info_ptr,\
				MMIQBTHEME_GetWallpaperSize(s_qbtheme_anim_cur_index - 1), is_update);
		}		
	}
}

/*******************************************************************/
//  Interface:		HandleAnimWallpaperGLideTpUp
//  Description : 	HandleAnimWallpaperGLideTpUp
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void HandleAnimWallpaperGLideTpUp(
										MMI_WIN_ID_T win_id, 
										uint16 item_max, 
										DPARAM param
										)
{
    int16           offset_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          i = 0;
    uint16          cur_index = 0;
    //GUI_POINT_T     tp_point = {0};
	
	offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
	
	if (!s_qbtheme_is_tp_panel
		&& 0 != offset_x
		&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
	{
        //get point
        //tp_point.x = MMK_GET_TP_X(param);
        //tp_point.y = MMK_GET_TP_Y(param);
		
		move_width = CalcMoveWidth(offset_x, item_max, QB_THEME_GLIDE_FRAME_GAP, &cur_index);
		
		//auto move
		if(0 != move_width)
		{
			for (i = 1; i <= QB_THEME_GLIDE_AUTO_FRAME; i++)
			{
				//set move space
				if (i <= (QB_THEME_GLIDE_AUTO_FRAME / 2))
				{
					move_offset = (int16)((2 * move_width * i * i) / 100);
				}
				else
				{
					move_offset = (int16)(move_width - (2 * move_width * (QB_THEME_GLIDE_AUTO_FRAME - i) * (QB_THEME_GLIDE_AUTO_FRAME - i)) / 100);
				}
				
				//set move space
				move_space = (int16)(move_offset - move_all);
				move_all   = (int16)(move_all + move_space);
				
				if (0 != move_space)
				{
					uint16 j = 0; 
					for (j = 0; j < item_max; j++)
					{
						SetAnimCtrlDisplayRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + j, move_space, TRUE);
					}
					
					//update
					MMITHEME_UpdateRect();
				}
			}
		}
		//
		if (cur_index != s_qbtheme_anim_cur_index)
		{
			BOOLEAN is_start = FALSE;
			BOOLEAN is_end = FALSE;
			QBTHEME_PACKAGE_INFO_T *info_ptr = MMIQBTHEME_GetCurThemeInfo();
			
			// modify title label
			s_qbtheme_anim_cur_index = cur_index;
			
			WallpaperSetUpLayerLable(win_id, info_ptr, item_max, TRUE);	
	
			is_start = 	(BOOLEAN)(0 == s_qbtheme_anim_cur_index);
			is_end = (BOOLEAN)(item_max - 1 == s_qbtheme_anim_cur_index);
			SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
				IMAGE_QBTHEME_BTN_RIGHT_GRAY, is_end, TRUE);
			SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
				IMAGE_QBTHEME_BTN_LEFT_GRAY, is_start, TRUE);
		}
		
		ResetPreviewStatus();
	}
}

/*******************************************************************/
//  Interface:		QBThemeWallpaperPreviewInit
//  Description : 	QBThemeWallpaperPreviewInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void QBThemeWallpaperPreviewInit(
									   MMI_WIN_ID_T  win_id, 
									   uint16 item_count
									   )
{
	GUI_BG_T   bg_btn	= {0};
	GUI_RECT_T rc_btn_l = QB_PANEL_LEFT_BTN_RECT;
	GUI_RECT_T rc_btn_r = QB_PANEL_RIGHT_BTN_RECT;
	GUI_RECT_T rc_name = QB_PANEL_NAME_LABEL_RECT;
	GUI_RECT_T rc_size = QB_PANEL_SIZE_LABEL_RECT;
	GUI_RECT_T rc_key_l = QB_PANEL_LEFT_KEY_RECT;
	GUI_RECT_T rc_key_r = QB_PANEL_RIGHT_KEY_RECT;
	uint16	   i = 0;
	QBTHEME_PACKAGE_INFO_T *pkg_info_ptr = MMIQBTHEME_GetCurThemeInfo();
	
	s_qbtheme_is_display_osd = TRUE;
	
	CreateIconLayer(win_id, FALSE);
	DisplayMenuIcon(win_id, &s_qbtheme_icon_layer);
	CreateOsdPanelLayer(win_id, TRUE);
	
	// left btn
	GUIBUTTON_SetRect(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, &rc_btn_l);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_LEFT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, PreQBWallpaper);
	GUIBUTTON_SetRunSheen(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// right btn
	GUIBUTTON_SetRect(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, &rc_btn_r);
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_QBTHEME_BTN_RIGHT_ON;	
	GUIBUTTON_SetPressedBg(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, &bg_btn);
	GUIBUTTON_SetCallBackFunc(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, NextQBWallpaper);
	GUIBUTTON_SetRunSheen(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	// label
	bg_btn.bg_type = GUI_BG_COLOR;
	bg_btn.color = MMI_TRANSPARENCE_COLOR;
	GUILABEL_SetBg(QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, &rc_name, FALSE);
	GUILABEL_SetFont(QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	GUILABEL_SetBg(QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, &bg_btn);
	GUILABEL_SetRect(QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, &rc_size, FALSE);
	GUILABEL_SetFont(QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, &s_qbtheme_up_panel_layer);
	
	WallpaperSetUpLayerLable(win_id, pkg_info_ptr, item_count, FALSE);
	
	// key left
	GUIBUTTON_SetRect(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, &rc_key_l);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, TXT_QB_SHOW_ICONS);
	GUIBUTTON_SetCallBackFunc(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, SetDispQBWallpaper);
	GUIBUTTON_SetRunSheen(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);
	
	// key right
	GUIBUTTON_SetRect(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, &rc_key_r);
#ifdef GUIF_SOFTKEY_PDASTYLE    
	bg_btn.bg_type = GUI_BG_IMG;
	bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;	
	GUIBUTTON_SetPressedBg(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, &bg_btn);
#endif    
	SetButtonFont(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, MMI_DEFAULT_NORMAL_FONT);
	GUIBUTTON_SetTextId(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, TXT_QB_THEME_APPLY);
	GUIBUTTON_SetCallBackFunc(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, ApplyQBWallpaper);
	GUIBUTTON_SetRunSheen(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, FALSE);
	MMK_SetCtrlLcdDevInfo(QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID, &s_qbtheme_down_panel_layer);

#if 0	
	// preview page
	for (i = 0; i < QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE; i++)
	{
		InitAnimCtrlRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + i, i - s_qbtheme_anim_cur_index);
	}
	
	//MMIQBTHEME_SetNeedTransform(FALSE);
	SetAnimCtrlParam(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID, IMAGE_THEME_BG, PNULL, TRUE, FALSE);
	//MMIQBTHEME_SetNeedTransform(TRUE);
	
	for (i = 0; i < item_count - 1; i++)
	{
		wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		
		MMIQBTHEME_GetWallpaerPreviewPath(pkg_info_ptr, i, anim_file);
		SetAnimCtrlParam(QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID + i, 0, anim_file,FALSE, FALSE);
	}
#else
	// preview page
	InitAnimCtrlRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID, 0 - s_qbtheme_anim_cur_index);
	SetAnimCtrlParam(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID, IMAGE_THEME_BG, PNULL, TRUE, FALSE);
	
	for (i = 0; i < QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE - 1; i++)
	{
		if (i < item_count - 1)
		{
			wchar anim_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
			
			MMIQBTHEME_GetWallpaerPreviewPath(pkg_info_ptr, i, anim_file);
			InitAnimCtrlRect(QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID + i, i + 1 - s_qbtheme_anim_cur_index);
			SetAnimCtrlParam(QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID + i, 0, anim_file, FALSE, FALSE);
		}
		else
		{
			MMK_DestroyControl(QBTHEME_WALLPAPER_PREVIEW2_ANIM_CTRL_ID + i);
		}
	}

#endif
	
	// is_start gray pre,is_end gray next
	if (0 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_LEFT, \
			IMAGE_QBTHEME_BTN_LEFT_GRAY, TRUE, FALSE);
	}
	if(item_count - 1 == s_qbtheme_anim_cur_index)
	{
		SetButtonGray(QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, IMAGE_QBTHEME_BTN_RIGHT, \
			IMAGE_QBTHEME_BTN_RIGHT_GRAY, TRUE, FALSE);
	}

    CreateReturnBtn(win_id, QBTHEME_WALLPAPER_PREVIEW_RETURN_BTN_CTRL_ID);
}

/*******************************************************************/
//  Interface:		HandleQbThemeWallpaperPreviewGlideWinMsg
//  Description : 	HandleQbThemeWallpaperPreviewGlideWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeWallpaperPreviewGlideWinMsg(
															MMI_WIN_ID_T      win_id, 	
															MMI_MESSAGE_ID_E  msg_id,
															DPARAM            param
															)
{
	MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id     = QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID;
	uint16				index		= (uint16)((uint32)MMK_GetWinAddDataPtr(win_id));
	uint32				item_count  = MMIQBTHEME_GetThemeWallpaperCount() + 1;
	
	if (item_count > QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE)
	{
		item_count = QB_THEME_WALLPAPER_PREVIEW_MAX_PAGE;
	}
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		s_qbtheme_anim_cur_index = index;
		MMIQBTHEME_SetNeedTransform(FALSE);
		QBThemeWallpaperPreviewInit(win_id, (uint16)item_count);
		MMK_SetActiveCtrl(ctrl_id + index, FALSE);
		break;
	case MSG_LOSE_FOCUS:
		SetOsdLayerDisplayStatus(FALSE);
        SetRetBtnLayerDisplay(FALSE);
		SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, FALSE);

        if(s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
        {
            int16 offset_x = s_qbtheme_tp_prev_pt.x - s_qbtheme_tp_start_pt.x;
            uint32			i = 0;
            for (i = 0; i < item_count; i++)
            {
                SetAnimCtrlDisplayRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + i, -offset_x, TRUE);
            }
            
            ResetPreviewStatus();
        }

		break;
	case MSG_GET_FOCUS:
		//SetIconLayerDisplayStatus(&s_qbtheme_icon_layer, TRUE);
		SetOsdLayerDisplayStatus(TRUE);
        SetRetBtnLayerDisplay(TRUE);
		GUIBUTTON_SetTextId(QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, TXT_QB_SHOW_ICONS);
		break;
	case MSG_FULL_PAINT:
		break;
	case MSG_NOTIFY_UPDATE:
		ClearOsdRectBg(win_id, *(GUI_RECT_T *)param);
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		HandleAnimGlideTpDown(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T     pt_move = {0};
			int16			offset_x = 0;
			uint32			i = 0;
			
			//get point
			pt_move.x = MMK_GET_TP_X(param);
			pt_move.y = MMK_GET_TP_Y(param);
			
			offset_x = (int16)(pt_move.x - s_qbtheme_tp_prev_pt.x);
			if (!s_qbtheme_is_tp_panel 
				&& 0 != offset_x
				&& QB_THEME_INVALID_TP_POINT != s_qbtheme_tp_start_pt.x)
			{
				GUI_RECT_T rc = MMITHEME_GetFullScreenRect();
				GUI_FillRect(MMITHEME_GetDefaultLcdDev(), rc, MMI_BACKGROUND_COLOR);
				
				for (i = 0; i < item_count; i++)
				{
					SetAnimCtrlDisplayRect(QBTHEME_WALLPAPER_PREVIEW1_ANIM_CTRL_ID + i, offset_x, TRUE);
				}
				
                if (UILAYER_IsBltLayer(&s_qbtheme_icon_layer))
				{
					SetOsdLayerDisplayStatus(FALSE);
					CopyOsdPanelLayer(&s_qbtheme_icon_layer, FALSE);
				}

				s_qbtheme_tp_prev_pt.x = pt_move.x;
				s_qbtheme_tp_prev_pt.y = pt_move.y;
				s_qbtheme_has_moved = TRUE;
				
				//MMITHEME_UpdateRect();
                
			}
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
		{
			if(!s_qbtheme_has_moved && !s_qbtheme_is_tp_panel)
			{
				HandlePanelLayerTpUp(QBTHEME_WALLPAPER_PREVIEW_NAME_LABEL_CTRL_ID, QBTHEME_WALLPAPER_PREVIEW_SIZE_LABEL_CTRL_ID, \
					QBTHEME_WALLPAPER_PREVIEW_LEFT_BTN_CTRL_ID, QBTHEME_WALLPAPER_PREVIEW_RIGHT_BTN_CTRL_ID, \
					QBTHEME_WALLPAPER_PREVIEW_DISP_BTN_CTRL_ID, QBTHEME_WALLPAPER_PREVIEW_APPLY_BTN_CTRL_ID);
			}
			else
			{
				HandleAnimWallpaperGLideTpUp(win_id, item_count, param);
                if (UILAYER_IsBltLayer(&s_qbtheme_icon_layer))
				{
					ClearOsdPanelLayer(&s_qbtheme_icon_layer);
					SetOsdLayerDisplayStatus(TRUE);
				}
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		MMIQBTHEME_SetNeedTransform(TRUE);		
		s_qbtheme_anim_cur_index = 0;
		DestroyIconLayer(&s_qbtheme_icon_layer);
		DestroyOsdPanelLayer();
        DestroyRetBtnLayer();
		ResetPreviewStatus();
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

//////////////////////////////////////////////////////////////////////////
//
/*******************************************************************/
//  Interface:		HandleQbQueryApplyWinMsg
//  Description : 	HandleQbQueryApplyWinMsg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E HandleQbQueryApplyWinMsg(
									   MMI_WIN_ID_T        win_id,    //IN:
									   MMI_MESSAGE_ID_E    msg_id,    //IN:
									   DPARAM              param    //IN:
									   )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *win_info_ptr   = PNULL;
    QBTHEME_PACKAGE_INFO_T* info_ptr = PNULL;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                info_ptr = (QBTHEME_PACKAGE_INFO_T*)(win_info_ptr->user_data);
            }
            
            MMIPUB_CloseQuerytWin(&win_id);
            if(PNULL != info_ptr)
            {
                ApplyTheme(info_ptr);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&win_id);
        break;
        
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_QueryApplyWin
//  Description : 	MMIAPIQBTHEME_QueryApplyWin
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_QueryApplyWin(
                                        QBTHEME_PACKAGE_INFO_T* info_ptr
                                        )
{
    MMI_WIN_ID_T query_win_id = MMIQBTHEME_QUERY_APPLY_WIN_ID;
    MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_QB_INSTRALL_SUCC, IMAGE_PUBWIN_QUERY,\
        &query_win_id, HandleQbQueryApplyWinMsg, (uint32)info_ptr);
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_OpenMainWin
//  Description : 	MMIAPIQBTHEME_OpenMainWin
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_OpenMainWin()
{
	MMK_CreateWin((uint32 *)QBTHEME_LIST_WIN_TAB, PNULL);
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_OpenWallpaperWin
//  Description : 	MMIAPIQBTHEME_OpenWallpaperWin
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIQBTHEME_OpenWallpaperWin()
{
	return MMK_CreateWin((uint32 *)QBTHEME_WALLPAPER_LIST_WIN_TAB, PNULL);
}


/*Edit by script, ignore 15 case. Thu Apr 26 19:01:04 2012*/ //IGNORE9527


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
