/*****************************************************************************
** File Name:      mmivp_menutable.h                                        *
** Author:                                                                   *
** Date:            2010-3-26                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to file manager module                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010-3         apple.zhang           Create
******************************************************************************/

#ifndef _MMIVP_MENUTABLE_H_
#define _MMIVP_MENUTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
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
//Menu ID
typedef enum
{
    MMIVP_ID_MENU_START = (MMI_MODULE_VP << 16),
    ID_VP_OPEN,
    ID_VP_VIEW,
    ID_VP_SETTING,
    ID_VP_FLOWRATE,
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
    ID_VP_HELP,
#endif
    ID_VP_OPEN_LOCALFILE,
    ID_VP_OPEN_NET,
    ID_VP_OPEN_LOCALLIST,
    ID_VP_VIEW_HISTORYLIST,
    ID_VP_VIEW_BOOKMARK,
    ID_VP_SET_REPEATE_MODE,
    ID_VP_SET_RANDOM_MODE,
    ID_VP_SET_NET,
    ID_VP_SET_NET1,
    ID_VP_SET_NET2,
    ID_VP_SET_NET3,
    ID_VP_SET_NET4,
	ID_VP_SET_PLAYBACK_MODE,
	ID_VP_AUDIO_VIDEO_SYN,
	ID_VP_SCREEN_FLOW,
    ID_VP_SET_RTCP_RTP,
    ID_VP_SET_AGENT,
    ID_VP_SET_RESET,
    ID_VP_FULLSCREEN,
    ID_VP_BRIGHTNESS,
    ID_VP_SAVE_BOOKMAR,
    ID_VP_SETTING_AS,
    ID_VP_FILEATTR,
    ID_VP_EXIT,
    ID_VP_RANDOM_OPEN,
    ID_VP_RANDOM_CLOSE,
    ID_VP_REPEAT_CLOSE,
    ID_VP_REPEAT_ALL,
    ID_VP_REPEAT_ONE,
    ID_VP_VIDEO_WALLPAPER,
    ID_VP_POWER_ON_SET,
    ID_VP_SCEENSAVE,
    ID_VP_POWER_OFF_SET,
    ID_VP_MUTE,
    ID_VP_OPERATER,
    ID_VP_DELETE,
    ID_VP_DELETE_CUR,
    ID_VP_DELETE_ALL,
    ID_VP_MODIFY,
    ID_VP_ADD,
    ID_VP_UPDATE,
    ID_VP_MARK_OPT,
    ID_VP_SUB_MARK,
    ID_VP_CANCEL_MARK,
    ID_VP_MARK_ALL,
    ID_VP_CANCEL_ALL_MARK,
    ID_VP_SET_PLAY_MODE,
    MMIVP_ID_MENU_MAX
}MMIFMM_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIVP_MENU_LABEL_START = (MMI_MODULE_VP << 16),
    #include "mmivp_menutable.def"
    MMIVP_MENUTABLE_MAX
} MMVP_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register vp menu group
//  Global resource dependence : none
//  Author: robert
//  Note:
/*****************************************************************************/
PUBLIC void MMIVP_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
