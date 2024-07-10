/*****************************************************************************
** File Name:      mmimp3_id.h                                               *
** Author:         haiyang.hu                                                *
** Date:           2006/09/22                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mp3 win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/22     haiayng.hu        Create                                   *
******************************************************************************/

#ifndef _MMIMP3_ID_H_
#define _MMIMP3_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    MMIMP3_WIN_ID_START = (MMI_MODULE_MP3 << 16),

#include "mmimp3_id.def"
#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
#include "audioplayer_demoapp_id.def"
#endif
    MMIMP3_MAX_WIN_ID
}MMIMP3_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIMP3_CTRL_ID_ID_START = MMIMP3_MAX_WIN_ID, //(MMI_MODULE_MP3 << 16),
    MMIPLAYER_MAIN_MENU_CTRL_ID,        //player main menu control id

    MMIMP3_MAIN_FORM_CTRL_ID,
    MMIMP3_BTN_FORM_CTRL1_ID,
    MMIMP3_BTN_FORM_FATHER_ID,
    MMIMP3_BTN_FORM_DUMMY_ID,
    MMIMP3_BTN_FORM_CTRL2_ID,
    MMIMP3_PROCESS_FORM_CTRL_ID,
    MMIMP3_TITLE_LABLE_CTRL_ID,
    MMIMP3_MAIN_OWNER_DRAW_CTRL_ID,
    MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID,
    MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID,
    MMIMP3_EQ_BTN_CTRL_ID,
    MMIMP3_REPEAT_BTN_CTRL_ID,
    MMIMP3_SHUFFLE_BTN_CTRL_ID,
    MMIMP3_VOLUME_BTN_CTRL_ID,
    MMIMP3_PREV_BTN_CTRL_ID,
    MMIMP3_PLAY_BTN_CTRL_ID,
    MMIMP3_NEXT_BTN_CTRL_ID,
    MMIMP3_REVIEW_BTN_CTRL_ID,
    MMIMP3_OPT_BTN_CTRL_ID,
    MMIMP3_RETRUN_BTN_CTRL_ID,


    MMIMP3_EQ_SET_LIST_CTRL_ID,         //eq set list control id
    MMIMP3_ALARM_RING_SET_LIST_CTRL_ID, //alarm ring set list control id
    MMIMP3_PLAY_MODE_LIST_CTRL_ID,      //play mode set list control id

    MMIMP3_PLAY_MODE_REPEAT_CTRL_ID,
    MMIMP3_PLAY_MODE_SHUFFLE_CTRL_ID,
    MMIMP3_DISPLAY_SET_CTRL_ID,

    MMIMP3_BT_SET_CTRL_ID,
    MMIMP3_BT_SET_FORM_CTRL_ID,
    MMIMP3_BT_SET_LABEL_CTRL_ID,

    MMIMP3_SET_TXT_CODE_TYPE_CTRL_ID,
    
    MMIMP3_MIAN_PLAY_ANIM_CTRL_ID,
    MMIMP3_LITTLE_PLAY_ANIM_CTRL_ID,

    MMIMP3_PLAYER_OPT_MENU_CTRL_ID,
    MMIMP3_PLAY_LIST_CTRL_ID, 
    MMIMP3_PLAY_LIST_OPT_MENU_CTRL_ID,
    MMIMP3_ALL_MP3_LIST_OPT_MENU_CTRL_ID,
    MMIMP3_CUR_PLAY_LIST_CTRL_ID,
    MMIMP3_CUR_PLAY_LIST_OPT_MENU_CTRL_ID,
    MMIMP3_PLAY_LIST_LIST_CTRL_ID,    
    MMIMP3_PLAY_LIST_LIST_OPT_MENU_CTRL_ID,
    MMIMP3_ADD_TO_LIST_CTRL_ID,
    MMIMP3_ADD_TO_LIST_FROM_CUR_PLAY_CTRL_ID,
    MMIMP3_ADD_TO_LIST_FROM_ALL_MP3_CTRL_ID,
    MMIMP3_CREATE_NEW_LIST_EDITBOX_CTRL_ID,
    MMIMP3_RENAME_LIST_EDITBOX_CTRL_ID,

    MMIMP3_MUSIC_MARK_LIST_CTRL_ID,
    MMIMP3_MUSIC_MARK_OPT_MENU_CTRL_ID,
    MMIMP3_RENAME_MARK_EDITBOX_CTRL_ID,

    MMIMP3_MUSIC_MARK_DETAIL_TEXT_CTRL_ID,
    MMIMP3_DETAIL_TEXT_CTRL_ID,
    
#ifdef MMI_PDA_SUPPORT
    MMIMP3_PLAY_LIST_FORM_CTRL_ID, //NEWMS00169534
    MMIMP3_MUSIC_MARK_LIST_FORM_CTRL_ID, 
    //context menu
    MMIMP3_PLAY_LIST_LIST_LONGOK_OPT_MENU_CTRL_ID,
    MMIMP3_PLAY_LIST_LONGOK_OPT_MENU_CTRL_ID,
    MMIMP3_MUSIC_MARK_LONGOK_OPT_MENU_CTRL_ID,
#endif
    //settings
    MMIMP3_SETTINGS_LIST_CTRL_ID,

    MMIMP3_LIST_BTN_CTRL_ID,
    MMIMP3_TITLE_FORM_CTRL_ID,
    MMIMP3_ARTIST_LABLE_CTRL_ID,
    MMIMP3_ALBUM_LABLE_CTRL_ID,
    MMIMP3_PANEL_H_FORM_CTRL_ID,        //ºáÆÁ¿ØÖÆÃæ°å
    MMIMP3_DUMY1_OWNER_DRAW_CTRL_ID,    //Ìî³ä
    MMIMP3_DUMY2_OWNER_DRAW_CTRL_ID,    //Ìî³ä
    MMIMP3_DUMY3_OWNER_DRAW_CTRL_ID,    //Ìî³ä
    MMIMP3_BTN_OWNER_DRAW_CTRL_ID,      //²¥·Åbutton²¼¾ÖÌî³ä£¬È·±£button´¹Ö±¾ÓÖÐ

    MMIMP3_PRE_ALBUM_ANIM_CTRL_ID,
    MMIMP3_MIAN_ALBUM_ANIM_CTRL_ID,
    MMIMP3_NEXT_ALBUM_ANIM_CTRL_ID,

    MMIMP3_FILE_INFO_LABLE_CTRL_ID,
    MMIMP3_OSD_PANEL_FORM_CTRL_ID,
    MMIMP3_OSD_BTN_FORM_CTRL_ID,

    //istyle
    MMIMP3_ISTYLE_RETURN_BTN_CTRL_ID,
    MMIMP3_ISTYLE_OPT_BTN_CTRL_ID,
    MMIMP3_ISTYLE_BTN_FORM_CTRL_ID,
    MMIMP3_ISTYLE_DUMY_OWNER_DRAW_CTRL_ID,

//#endif  //MMI_PDA_SUPPORT
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
	MMIMP3_DUMMY_LABLE_CTRL_ID,
	MMIMP3_PIC_BTN_CTRL_ID,
#endif

    MMIMP3_MAX_CTRL_ID
}MMIMP3_CONTROL_ID_E;
#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
#include "audioplayer_demoapp_id.h"
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIMP3_ID_H_
