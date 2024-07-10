/*****************************************************************************
** File Name:      mmimp3_menutable.h                                        *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine           Create
******************************************************************************/

#ifndef _MMIMP3_MENUTABLE_H_
#define _MMIMP3_MENUTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmifmm_export.h"

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
//Menu ID
typedef enum
{
    MMIMP3_ID_MENU_START = (MMI_MODULE_MP3 << 16),

    ID_PLAYER_MP3,
    ID_PLAYER_VIDEO,
    ID_PLAYER_FM,
//macro replace
//#ifdef MMI_STREAMING_ENABLE 
#ifdef STREAMING_SUPPORT_NEXTREAMING 
    ID_PLAYER_ST,
#endif  
#ifdef KURO_SUPPORT
    ID_PLAYER_KUR,
#endif
    ID_PLAYER_MTV,

    //opt menu
    ID_MP3_PLAYER_OPT_START,
    //ID_MP3_PLAYER_OPT_RETURN = ID_MP3_PLAYER_OPT_START,  //RETURN_TO_MAIN_WIN
    ID_MP3_MODE = ID_MP3_PLAYER_OPT_START,
    ID_MP3_PLAY_LIST_LIST,
    ID_MP3_MUSIC_MARK_MENU,
    ID_MP3_ADD,
    ID_MP3_SET_RING,
    ID_MP3_SET_REPEAT,
    ID_MP3_SET_SHUFFLE,
    ID_MP3_SETTINGS,
    ID_MP3_REVIEW,
    ID_MP3_EXIT,
    ID_MP3_PLAYER_OPT_END = ID_MP3_EXIT,

    //add menu
    ID_MP3_ADD_MENU_START,
    ID_MP3_ADD_TO_PLAY_LIST = ID_MP3_ADD_MENU_START,
    ID_MP3_ADD_TO_NEW_LIST,
    ID_MP3_ADD_TO_MUSIC_MARK,


    //set call ring
    ID_MP3_SET_RING_START,
    ID_MP3_SET_CALL_RING = ID_MP3_SET_RING_START,
    ID_MP3_SET_ALARM_RING,
    ID_MP3_SET_RING_END = ID_MP3_SET_ALARM_RING,

    //setting menu
    ID_MP3_SETTING_START,
    ID_MP3_EQ_SET = ID_MP3_SETTING_START,
    //ID_MP3_SET_LYRIC,
    //ID_MP3_SPECIAL_VIEW,
    ID_MP3_DISPLAY_SET,
#ifdef BLUETOOTH_SUPPORT
    ID_MP3_BT_SET,
    ID_MP3_BT_HEADSET_LIST,
#endif

#if defined BIG5_SUPPORT
    ID_MP3_CODE_TYPE,
#endif
    ID_MP3_SETTING_END = ID_MP3_DISPLAY_SET,

    //list list opt
    ID_MP3_LIST_LIST_OPT_START,
    ID_MP3_LIST_LIST_DEL = ID_MP3_LIST_LIST_OPT_START,  
    ID_MP3_LIST_LIST_NEW,
    ID_MP3_LIST_LIST_RENAME,
    ID_MP3_LIST_LIST_COPY,
    //ID_MP3_LIST_LIST_COPY_1,
    ID_MP3_LIST_LIST_OPT_END = ID_MP3_LIST_LIST_COPY + MMI_SDCARD_MAX_NUM,

    //play list opt menu
    ID_MP3_LIST_OPT_START,
    ID_MP3_LIST_OPT_RETURN = ID_MP3_LIST_OPT_START,  //RETURN_TO_MAIN_WIN
    ID_MP3_LIST_ADD,
    ID_MP3_LIST_SEARCH_ALL,
    ID_MP3_LIST_DEL, 
    ID_MP3_LIST_DEL_ALL,
    ID_MP3_LIST_MARK,
    //ID_MP3_LIST_SET_RING,
    ID_MP3_LIST_SEND,
    ID_MP3_LIST_DETAIL, 
    //ID_MP3_LIST_ADD_TO,
#ifdef MMIAP_SORT_SUPPORT      
    ID_MP3_LIST_SORT,
#endif

    //my music list opt menu
    ID_MP3_MY_MUSIC_OPT_START,
    ID_MP3_MY_MUSIC_OPT_RETURN = ID_MP3_MY_MUSIC_OPT_START,  //RETURN_TO_MAIN_WIN
    ID_MP3_MY_MUSIC_DEL_REAL, 
    ID_MP3_MY_MUSIC_MARK,
    ID_MP3_MY_MUSIC_SET_RING,
    ID_MP3_MY_MUSIC_SEND,
    ID_MP3_MY_MUSIC_DETAIL, 
    ID_MP3_MY_MUSIC_ADD_TO,
#ifdef MMIAP_SORT_SUPPORT   
    ID_MP3_MY_MUSIC_SORT,
    ID_MP3_MY_MUSIC_OPT_END = ID_MP3_MY_MUSIC_SORT,
#else    
    ID_MP3_MY_MUSIC_OPT_END = ID_MP3_MY_MUSIC_ADD_TO,
#endif
    //list delete menu
    ID_MP3_LIST_DEL_MENU_START,
    ID_MP3_LIST_DEL_FROM_LIST = ID_MP3_LIST_DEL_MENU_START, 
    ID_MP3_LIST_DEL_REAL,
    ID_MP3_LIST_DEL_MENU_END = ID_MP3_LIST_DEL_REAL,

    //list mark menu
    ID_MP3_LIST_MARK_MENU_START,
    ID_MP3_SUB_MARK = ID_MP3_LIST_MARK_MENU_START, 
    ID_MP3_CANCEL_MARK,
    ID_MP3_MARK_ALL,
    ID_MP3_CANCEL_ALL_MARK,
    ID_MP3_LIST_MARK_MENU_END = ID_MP3_CANCEL_ALL_MARK,

    //list add to menu
    ID_MP3_LIST_ADD_MENU_START,
    ID_MP3_LIST_ADD_TO_LIST = ID_MP3_LIST_ADD_MENU_START, 
    //ID_MP3_LIST_ADD_TO_NEW_LIST,
    //ID_MP3_LIST_ADD_MENU_END = ID_MP3_LIST_ADD_TO_NEW_LIST,

    //sort menu
    ID_MP3_LIST_SORT_START,
    ID_MP3_LIST_SORT_BY_NONE = ID_MP3_LIST_SORT_START,   //速度优先
    ID_MP3_LIST_SORT_BY_TIME,
    ID_MP3_LIST_SORT_BY_SIZE,
    ID_MP3_LIST_SORT_BY_NAME,
    ID_MP3_LIST_SORT_END = ID_MP3_LIST_SORT_BY_NAME,

    ID_MP3_LIST_SORT_BY_TYPE,

    //send menu
    ID_MP3_SEND_MENU_START,
    ID_MP3_SEND_BY_MMS = ID_MP3_SEND_MENU_START,
    ID_MP3_SEND_BY_BT,
    ID_MP3_SEND_MENU_END = ID_MP3_SEND_BY_BT,

    //mark opt menu
    ID_MP3_MARK_OPT_START,
    //ID_MP3_MARK_PLAY,
    ID_MP3_MARK_ADD = ID_MP3_MARK_OPT_START,
    //ID_MP3_MARK_DEL,
    //ID_MP3_MARK_MARK,
    //ID_MP3_MARK_RENAME,
    //ID_MP3_MARK_DETAIL,
    //ID_MP3_MARK_OPT_END = ID_MP3_MARK_DETAIL,

    //lyric set
    ID_MP3_LYRIC_OPT_START,
    ID_MP3_LYRIC_ON_WITH_BACKLIGHT = ID_MP3_LYRIC_OPT_START,
    ID_MP3_LYRIC_ON,
    ID_MP3_LYRIC_OFF,
    ID_MP3_LYRIC_OPT_END = ID_MP3_LYRIC_OFF,

    //repeat menu
    ID_MP3_REPEAT_MENU_START,
    ID_MP3_REPEAT_ON = ID_MP3_REPEAT_MENU_START,
    ID_MP3_REPEAT_ONE_ON,
    ID_MP3_REPEAT_OFF,
    ID_MP3_REPEAT_MENU_END = ID_MP3_REPEAT_OFF,

    //shuffle menu
    ID_MP3_SHUFFLE_MENU_START,
    ID_MP3_SHUFFLE_ON = ID_MP3_SHUFFLE_MENU_START,
    ID_MP3_SHUFFLE_OFF,
    ID_MP3_SHUFFLE_MENU_END = ID_MP3_SHUFFLE_OFF,

//#ifdef MMI_PDA_SUPPORT
    //display set sub menu
    ID_MP3_DISPLAY_ALBUM, //专辑
#ifdef MMIAP_LYRIC_SUPPORT    
    ID_MP3_DISPLAY_LYRIC_ON,
    ID_MP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT,
#endif    
    ID_MP3_DISPLAY_SPECTRUM,
    ID_MP3_DISPLAY_ANIM,
//#endif

    MMIMP3_ID_MENU_MAX
}MMIMP3_ID_MENU_E;

#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
#include "audioplayer_demoapp_menutable.h"
#endif

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIMP3_MENU_LABEL_START = (MMI_MODULE_MP3 << 16),
    #include "mmimp3_menutable.def"
#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
    #include "audioplayer_demoapp_menutable.def"
#endif
    MMIMP3_MENUTABLE_MAX
} MMIMP3_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register mp3 menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
