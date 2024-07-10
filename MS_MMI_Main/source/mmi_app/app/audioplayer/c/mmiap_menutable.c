/*****************************************************************************
** File Name:      mmimp3_menutable.c                                        *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine.Meng       Create                                  *
******************************************************************************/

#define _MMIMP3_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guimenu.h"
#include "mmimp3_menutable.h"
#include "mmi_common.h"
#include "mmimp3_text.h"
//macro replace
//#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
#include "mmist_text.h"
#endif
#ifdef KURO_SUPPORT
#include "mmikuro_text.h"
#endif

#include "mmialarm_text.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_text.h"
#endif
#include "mmifmm_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL    0

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
//player
static const GUIMENU_ITEM_T s_menu_player[] =
{
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    {ID_PLAYER_MP3,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_AUDIO,                 IMAGE_SECMENU_ICON_MULTIM_MP3,  0, 0, NULL},
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    {ID_PLAYER_VIDEO,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VIDEO, IMAGE_SECMENU_ICON_MULTIM_MEDIA, 0, 0, NULL},
#endif
#endif
#if defined FM_SUPPORT && !defined MMI_SECMENU_ICON_DEFAULT
    {ID_PLAYER_FM,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_FM,                  IMAGE_SECMENU_ICON_MULTIM_FM, 0, 0, NULL},
#endif
    //macro replace
    //#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
    {ID_PLAYER_ST,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_ST_STREAMING,        IMAGE_SECMENU_ICON_MULTIM_STREAM, 0, 0, NULL},
#endif
#ifdef KURO_SUPPORT
    {ID_PLAYER_KUR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KUR_PLAYER,         IMAGE_SECMENU_ICON_MULTIM_KURO, 0, 0, NULL},
#endif
#ifdef CMMB_SUPPORT
    {ID_PLAYER_MTV,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MTV_MOBILETV,          IMAGE_SECMENU_ICON_MULTIM_MTV, 0, 0, NULL},
#endif

#if !((defined MMI_AUDIO_PLAYER_SUPPORT) || (defined VIDEO_PLAYER_SUPPORT) || (defined FM_SUPPORT)  || (defined STREAMING_SUPPORT_NEXTREAMING) || (defined KURO_SUPPORT)  || (defined CMMB_SUPPORT))
    {0}
#endif
};
#endif

#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
static const GUIMENU_ITEM_T audioplayer_demoapp_type_menu[] =
{
    {ID_AUDIOPLAYER_DEMOAPP_NETWORK_MUSIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_AUDIOPLAYER_DEMOAPP_NETWORK_MUSIC, 0, 0, 0, NULL},
    {ID_AUDIOPLAYER_DEMOAPP_LOCAL_MUSIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_AUDIOPLAYER_DEMOAPP_LOCAL_MUSIC, 0, 0, 0, NULL}
};
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
                 _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT,


static const GUIMENU_GROUP_T s_mp3_menu_table[] =
{
    {NULL}
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
,
#include "mmimp3_menutable.def"
#endif
#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
,
#include "audioplayer_demoapp_menutable.def"
#endif
};

#undef MENU_DEF
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
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : register mp3 menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_MP3, s_mp3_menu_table);
}

